# --- local_server.ps1 ---
# PowerShell local server with WASM range support

$port = 8080
$baseUrl = "http://localhost:$port/"
$sessionId = [guid]::NewGuid().ToString()
$fullUrl = $baseUrl + "?session=" + $sessionId

$listener = New-Object System.Net.HttpListener
$listener.Prefixes.Add("http://localhost:$port/")
$listener.Prefixes.Add("http://127.0.0.1:$port/")
$listener.Prefixes.Add("http://[::1]:$port/")
$listener.Start()

Start-Sleep -Milliseconds 300

Write-Host ("Full URL: " + $fullUrl)
Start-Process $fullUrl
Write-Host ("Local server running at " + $baseUrl)
Write-Host ("Serving from: " + (Get-Location))
Write-Host ""
Write-Host "Press Ctrl+C to stop the server manually, or close the browser tab to send a close signal."
Write-Host ""

try {
    while ($listener.IsListening) {
        $context = $listener.GetContext()
        $req = $context.Request
        $res = $context.Response

        # Always add CORS for WASM
        $res.AddHeader("Access-Control-Allow-Origin", "*")

        $path = $req.Url.LocalPath.TrimStart('/')
        if ([string]::IsNullOrWhiteSpace($path)) { $path = "app.html" }

        $file = Join-Path (Get-Location) $path

        if (Test-Path $file) {
            Write-Host "Serving file: $file"

            $bytes = [System.IO.File]::ReadAllBytes($file)
            $res.ContentType = switch -Regex ($file) {
                '\.html?$' { 'text/html' }
                '\.js$'    { 'application/javascript' }
                '\.wasm$'  { 'application/wasm' }
                '\.css$'   { 'text/css' }
                '\.png$'   { 'image/png' }
                '\.jpg$'   { 'image/jpeg' }
                '\.gif$'   { 'image/gif' }
                '\.svg$'   { 'image/svg+xml' }
                default    { 'application/octet-stream' }
            }

            # Handle Range requests for WASM
            $rangeHeader = $req.Headers["Range"]
            if ($rangeHeader -and $file -match '\.wasm$') {
                if ($rangeHeader -match "bytes=(\d+)-(\d*)") {
                    $start = [int64]$matches[1]
                    $end = if ($matches[2]) { [int64]$matches[2] } else { $bytes.Length - 1 }
                    $length = $end - $start + 1
                    $res.StatusCode = 206
                    $res.AddHeader("Content-Range", "bytes $start-$end/$($bytes.Length)")
                    $res.ContentLength64 = $length
                    $res.OutputStream.Write($bytes, $start, $length)
                    $res.OutputStream.Close()
                    continue
                }
            }

            # Regular full file response
            $res.ContentLength64 = $bytes.Length
            $res.OutputStream.Write($bytes, 0, $bytes.Length)
        }
        else {
            Write-Host "404 Not Found: $file"
            $res.StatusCode = 404
            $msg = [System.Text.Encoding]::UTF8.GetBytes("404 Not Found: " + $path)
            $res.ContentType = "text/plain"
            $res.ContentLength64 = $msg.Length
            $res.OutputStream.Write($msg, 0, $msg.Length)
        }

        $res.OutputStream.Close()
    }
}
finally {
    if ($listener -and $listener.IsListening) {
        $listener.Stop()
    }
    Write-Host "Server stopped."
}
