const canvas = document.getElementById("gameCanvas");
const ctx = canvas.getContext("2d");

function resizeCanvas() {
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
}
resizeCanvas();
window.addEventListener("resize", resizeCanvas);

Module.onRuntimeInitialized = () => {
  const update = Module.cwrap("update", null, []);
  const getX = Module.cwrap("get_x", "number", []);
  const right_movement = Module.cwrap("right_movement", "number", []);
  const left_movement = Module.cwrap("left_movement", "number", []);
  const shoot_bullet = Module.cwrap("shoot_bullet", "number", []);
  const count = Module.cwrap("get_bullet_count", "number", []);

  
  const scale = 8;
  const shipPattern = [
    [0,0,0,1,0,0,0],
    [0,0,1,1,1,0,0],
    [0,1,1,1,1,1,0],
    [1,1,0,1,0,1,1],
    [1,1,1,1,1,1,1],
    [0,1,1,0,1,1,0],
    [0,0,1,0,1,0,0]
  ];

  function loop() {
    handleInput()
    update();
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    //Ship Rendering
    ctx.fillStyle = 'rgba(255, 255, 255, 1)';
    const shipX = getX(); // horizontal offset from WASM
    const shipY = canvas.height - 160; // put near bottom

    for (let y = 0; y < shipPattern.length; y++) {
      for (let x = 0; x < shipPattern[y].length; x++) {
        if (shipPattern[y][x]) {
          ctx.fillRect(shipX + x * scale, shipY + y * scale, scale, scale);
        }
      }
    }

    //Bullet Rendering
    for (let i = 0; i < count(); i++) {
      const x = Module._get_bullet_x(i);
      const y = Module._get_bullet_y(i);
      ctx.fillRect(x, y, 8, 16);
    }

    //Komsai Rendering
    // (Not implemented yet)
    requestAnimationFrame(loop);
  }

  const keysPressed = {};
  let lastShotTime = 0;
  const SHOOT_COOLDOWN = 300;

  document.addEventListener("keydown", (event) => {
    keysPressed[event.key] = true;
  });

  document.addEventListener("keyup", (event) => {
    keysPressed[event.key] = false;
  });

  function handleInput() {
    if (keysPressed["ArrowRight"]) right_movement();
    if (keysPressed["ArrowLeft"]) left_movement();

    const now = Date.now();
    if (keysPressed[" "] && now - lastShotTime >= SHOOT_COOLDOWN) {
      shoot_bullet();
      lastShotTime = now;
    }
  }

  loop();
};