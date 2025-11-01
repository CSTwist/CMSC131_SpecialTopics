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
  const right_movement = Module.cwrap("right_movement", null, []);
  const left_movement = Module.cwrap("left_movement", null, []);
  const shoot_bullet = Module.cwrap("shoot_bullet", null, []);
  const bullet_count = Module.cwrap("get_bullet_count", "number", []);
  const komsai_count = Module.cwrap("get_komsai_count", "number", []);
  const generate_komsai = Module.cwrap("generate_komsai", null, []);

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

  //Komsai Image
  const img = new Image();
  img.src = "assets/images/Hew2.png";
  let lastKomsaiSpawn = 0;
  const SPAWN_COOLDOWN = 1000;
  img.onload = function(){
    loop();
  }

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
    for (let i = 0; i < bullet_count(); i++) {
      const x = Module._get_bullet_x(i);
      const y = Module._get_bullet_y(i);
      ctx.fillRect(x, y, 8, 16);
    }

    //Komsai Rendering
    spawnKomsai();
    for (let i = 0; i < komsai_count(); i++) {
      const x = Module._get_komsai_x(i);
      const y = Module._get_komsai_y(i);
      ctx.drawImage(img, x, y, 56, 56);
    }

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

  function spawnKomsai() {
    const now = Date.now();
    if (now - lastKomsaiSpawn >= SPAWN_COOLDOWN) {
      generate_komsai();
      lastKomsaiSpawn = now;
    }
  }
};