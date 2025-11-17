const starCanvas = document.getElementById('starfieldCanvas');
const sCtx = starCanvas.getContext('2d');
const STAR_COUNT = 180;
const stars = [];
const mouse = { x: window.innerWidth / 2, y: window.innerHeight / 2 };

function resizeStarCanvas() {
  starCanvas.width = window.innerWidth;
  starCanvas.height = window.innerHeight;
}

window.addEventListener('resize', resizeStarCanvas);
resizeStarCanvas();

window.addEventListener('mousemove', (e) => {
  mouse.x = e.clientX;
  mouse.y = e.clientY;
});

const colors = [
  'rgba(255,255,255,',
  'rgba(200,220,255,',
  'rgba(255,255,200,',
  'rgba(255,200,255,',
  'rgba(180,255,255,'
];

for (let i = 0; i < STAR_COUNT; i++) {
  stars.push({
    x: Math.random() * starCanvas.width,
    y: Math.random() * starCanvas.height,
    size: Math.random() * 2 + 0.5,
    baseOpacity: Math.random() * 0.5 + 0.5,
    color: colors[Math.floor(Math.random() * colors.length)],
    speedY: Math.random() * 0.3 + 0.1,
    twinkleSpeed: Math.random() * 0.02 + 0.01,
    phase: Math.random() * Math.PI * 2,
    offsetX: 0,
    offsetY: 0
  });
}

function drawStars() {
  sCtx.clearRect(0, 0, starCanvas.width, starCanvas.height);
  stars.forEach(star => {
    const twinkle = Math.sin(Date.now() * star.twinkleSpeed + star.phase) * 0.5 + 0.5;
    const opacity = Math.min(1, star.baseOpacity * (0.7 + twinkle * 0.8));

    sCtx.beginPath();
    sCtx.arc(star.x + star.offsetX, star.y + star.offsetY, star.size, 0, Math.PI * 2);
    sCtx.fillStyle = `${star.color}${opacity})`;
    sCtx.shadowBlur = 15;
    sCtx.shadowColor = "white";
    sCtx.fill();
    sCtx.shadowBlur = 0;
  });
}

function updateStars() {
  stars.forEach(star => {
    star.y += star.speedY;
    if (star.y > starCanvas.height) {
      star.y = 0;
      star.x = Math.random() * starCanvas.width;
    }

    const dx = mouse.x - (star.x + star.offsetX);
    const dy = mouse.y - (star.y + star.offsetY);
    const dist = Math.sqrt(dx * dx + dy * dy);
    const influence = Math.max(0, 150 - dist) / 150;
    star.offsetX += dx * influence * 0.03;
    star.offsetY += dy * influence * 0.03;

    star.offsetX *= 0.95;
    star.offsetY *= 0.95;
  });
}

function animateStars() {
  updateStars();
  drawStars();
  requestAnimationFrame(animateStars);
}

animateStars();

const mainMenu = document.getElementById('main-menu');
const instructionsScreen = document.getElementById('instructions-screen');
const startSound = new Audio("assets/music/start.mp3");
startSound.volume = 0.5;
startSound.loop = true;
let musicStarted = false;

window.addEventListener('click', () => {
  if (!musicStarted) { startSound.play().catch(()=>{}); musicStarted = true; }
});

window.addEventListener('keydown', (event) => {
  if (!musicStarted) { startSound.play().catch(()=>{}); musicStarted = true; }
  if (event.code === 'Enter') {
    if (mainMenu.style.display !== 'none') {
      mainMenu.style.display = 'none';
      instructionsScreen.style.display = 'flex';
    } else if (instructionsScreen.style.display !== 'none') {
      instructionsScreen.style.display = 'none';
      document.querySelector('.game-screen').style.display = 'flex';
      document.querySelector('.SHOOT_THE_KOMSAI_Canvas').style.display = 'flex';
      startGame();
    }
  }
});
  