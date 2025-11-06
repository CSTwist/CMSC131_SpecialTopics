# --- local_server.ps1 ---
# Cleanest possible ASCII version

$port = Get-Random -Minimum 8000 -Maximum 9000
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

        $path = $req.Url.LocalPath.TrimStart('/')

        if ($path -eq "close") {
            Write-Host "Close signal received - stopping server..."
            $res.StatusCode = 200
            $msg = [System.Text.Encoding]::UTF8.GetBytes("Server closing")
            $res.ContentType = "text/plain"
            $res.ContentLength64 = $msg.Length
            $res.OutputStream.Write($msg, 0, $msg.Length)
            $res.OutputStream.Close()
            break
        }

        if ([string]::IsNullOrWhiteSpace($path)) { $path = "app.html" }
        $file = Join-Path (Get-Location) $path

        if (Test-Path $file) {
            $bytes = [System.IO.File]::ReadAllBytes($file)
            switch -Regex ($file) {
                '\.html?$' { $res.ContentType = 'text/html' }
                '\.js$'    { $res.ContentType = 'application/javascript' }
                '\.wasm$'  { $res.ContentType = 'application/wasm' }
                '\.css$'   { $res.ContentType = 'text/css' }
                '\.png$'   { $res.ContentType = 'image/png' }
                '\.jpg$'   { $res.ContentType = 'image/jpeg' }
                '\.gif$'   { $res.ContentType = 'image/gif' }
                '\.svg$'   { $res.ContentType = 'image/svg+xml' }
                default    { $res.ContentType = 'application/octet-stream' }
            }
            $res.ContentLength64 = $bytes.Length
            $res.OutputStream.Write($bytes, 0, $bytes.Length)
        }
        else {
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
