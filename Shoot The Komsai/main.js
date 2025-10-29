const canvas = document.getElementById("gameCanvas");
const ctx = canvas.getContext("2d");

function resizeCanvas() {
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
}

// Set size at start
resizeCanvas();

// Update size if the window is resized
window.addEventListener("resize", resizeCanvas);

// Wait until WASM is loaded
Module.onRuntimeInitialized = () => {
  const update = Module.cwrap("update", null, []);
  const getX = Module.cwrap("get_x", "number", []);
  const right_movement = Module.cwrap("right_movement", "number", []);
  const left_movement = Module.cwrap("left_movement", "number", []);
  
  const img = new Image();
  img.src = "assets/images/Hew2.png";
  img.onload = function() {
      loop();
  };

  function loop() {
    update();

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.beginPath();
    ctx.drawImage(img, getX(), window.innerHeight-100, 100, 100);

    requestAnimationFrame(loop);
  }
  document.addEventListener("keydown", function(event) {
    if (event.key === "ArrowRight") right_movement();
    if (event.key === "ArrowLeft") left_movement();
  });
};