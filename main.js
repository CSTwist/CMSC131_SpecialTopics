// --- STARFIELD LOGIC ---
(function initStarfield() {
    const canvas = document.getElementById("starfieldCanvas");
    const ctx = canvas.getContext("2d");
    let width, height;
    const stars = [];
    const numStars = 150;

    window.starSpeedMultiplier = 1.0; 

    function resize() {
        width = window.innerWidth;
        height = window.innerHeight;
        canvas.width = width;
        canvas.height = height;
    }
    window.addEventListener("resize", resize);
    resize();

    for(let i=0; i<numStars; i++) {
        stars.push({
            x: Math.random() * width,
            y: Math.random() * height,
            size: Math.random() * 2,
            speed: Math.random() * 5 + 0.5
        });
    }

    function animateStars() {
        ctx.clearRect(0, 0, width, height);
        ctx.fillStyle = "white";
        stars.forEach(star => {
            ctx.globalAlpha = Math.random() * 0.5 + 0.3;
            ctx.fillRect(star.x, star.y, star.size, star.size);
            star.y += star.speed * window.starSpeedMultiplier;
            if (star.y > height) {
                star.y = 0;
                star.x = Math.random() * width;
            }
        });
        requestAnimationFrame(animateStars);
    }
    animateStars();
})();

// --- GAME LOGIC ---
window.startGame = function () {
    const canvas = document.getElementById("gameCanvas");
    const ctx = canvas.getContext("2d");

    function resizeCanvas() {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    resizeCanvas();
    window.addEventListener("resize", resizeCanvas);

    // --- SFX SETUP ---
    const sfxExplosion = new Audio("assets/music/boom2.wav");
    const sfxDamage = new Audio("assets/music/boom11.wav");
    const sfxHeal = new Audio("assets/music/sound3.wav");
    const sfxAlarm = new Audio("assets/music/alarm sound.mp3");
    const startSound = new Audio("assets/music/shoot_sound.mp3");
    const sfxBossShoot = new Audio("assets/music/shoot_sound.mp3");

    const bgMusic = document.getElementById("bgMusic");

    sfxAlarm.loop = true;
    sfxAlarm.volume = 0.5;
    sfxExplosion.volume = 0.6;
    sfxDamage.volume = 0.8;
    sfxHeal.volume = 0.8;
    startSound.volume = 1;
    sfxBossShoot.volume = 1; 

    if(bgMusic) bgMusic.volume = 0.3;

    // --- FLOATING TEXT CLASS ---
    class FloatingText {
        constructor(x, y, text, color) {
            this.x = x;
            this.y = y;
            this.text = text;
            this.color = color;
            this.velocity = -2;
            this.alpha = 1;
            this.life = 50;
        }
        update() {
            this.y += this.velocity;
            this.alpha -= 0.02;
            this.life--;
        }
        draw(ctx) {
            ctx.save();
            ctx.globalAlpha = Math.max(0, this.alpha);
            ctx.fillStyle = this.color;
            ctx.font = "60px 'Pixelify Sans'";
            ctx.shadowColor = this.color;
            ctx.shadowBlur = 5;
            ctx.fillText(this.text, this.x, this.y);
            ctx.restore();
        }
    }
    let floatingTexts = [];

    // --- PARTICLE SYSTEM ---
    let particles = [];
    class Particle {
        constructor(x, y, color) {
            this.x = x;
            this.y = y;
            const angle = Math.random() * Math.PI * 2;
            const speed = Math.random() * 6 + 2; 
            this.vx = Math.cos(angle) * speed;
            this.vy = Math.sin(angle) * speed;
            this.life = 1.0;
            this.color = color;
            this.size = Math.random() * 6 + 4; 
            this.friction = 0.96; 
            this.gravity = 0.1;   
        }
        update() {
            this.vx *= this.friction;
            this.vy *= this.friction;
            this.vy += this.gravity;
            this.x += this.vx;
            this.y += this.vy;
            this.life -= 0.015;
            this.size *= 0.95; 
        }
        draw(ctx) {
            ctx.globalAlpha = Math.max(0, this.life);
            ctx.fillStyle = this.color;
            ctx.beginPath();
            ctx.arc(this.x, this.y, Math.max(0, this.size), 0, Math.PI * 2);
            ctx.fill();
            ctx.globalAlpha = 1.0;
        }
    }

    function createExplosion(x, y, color="orange") {
        for(let i=0; i<40; i++) {
            particles.push(new Particle(x, y, color));
        }
    }

    // --- SHAKE LOGIC ---
    function triggerShake(intensity) {
        document.body.classList.remove('shake-small', 'shake-medium', 'shake-large');
        void document.body.offsetWidth;
        const shakeClass = `shake-${intensity}`;
        document.body.classList.add(shakeClass);
        setTimeout(() => document.body.classList.remove(shakeClass), 500);
    }

    let isPaused = false;
    let previousLife = 5;
    let previousScore = 0;
    
    // Combo System
    let comboCount = 0;
    let comboTimer = 0;
    const COMBO_MAX_TIME = 2000; 

    // Level & Boss State
    let lastKnownLevel = -1;
    let levelStartTime = 0;
    let bossSpawned = false;
    // [FIXED] Initialize to -1 to ensure shuffle happens on Level 1 start
    let lastShuffledLevel = -1; 
    const BOSS_MAX_HP = 20;

    function initWhenModuleReady() {
        if (!Module || !Module.calledRun) {
            setTimeout(initWhenModuleReady, 50);
            return;
        }

        const update = Module.cwrap("update", null, ["number"]);
        const getX = Module.cwrap("get_x", "number", []);
        const right_movement = Module.cwrap("right_movement", null, []);
        const left_movement = Module.cwrap("left_movement", null, []);
        const shoot_bullet = Module.cwrap("shoot_bullet_player", null, []);
        const shoot_bullet_boss = Module.cwrap("shoot_bullet_boss", null, []);
        const bullet_count = Module.cwrap("get_bullet_count", "number", []);
        const komsai_count = Module.cwrap("get_komsai_count", "number", []);
        const generate_komsai = Module.cwrap("generate_komsai", null, []);
        const get_komsai_type = Module.cwrap("get_komsai_type", "number", ["number"]);
        const get_score = Module.cwrap("get_score", "number", []);
        const get_player_life = Module.cwrap("get_player_life", "number", []);
        const get_game_level = Module.cwrap("get_game_level", "number", []);
        const spawnBoss = Module.cwrap("spawn_boss", null, []);
        const get_boss_x = Module.cwrap("get_boss_x", "number", ["number"]);
        const get_boss_y = Module.cwrap("get_boss_y", "number", ["number"]);
        const get_boss_health = Module.cwrap("get_boss_health", "number", ["number"]);

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

        // Images
        const explosionImg = new Image();
        explosionImg.src = "assets/images/explosion.png";
        
        const bossImg = new Image();
        bossImg.src = "assets/images/ERYLBOSS.png";

        const images = [
            "assets/images/Chakie.png", 
            "assets/images/Hew2.png",   
            "assets/images/Soph.png",
            "assets/images/RJ.png",
            "assets/images/Christian.png"
        ];
        const komsaiImages = images.map(src => {
            const img = new Image();
            img.src = src;
            return img;
        });

        // Helper to shuffle images
        let shuffledImages = [...komsaiImages]; 
        
        function shuffleKomsaiImages() {
            for (let i = shuffledImages.length - 1; i > 0; i--) {
                const j = Math.floor(Math.random() * (i + 1));
                [shuffledImages[i], shuffledImages[j]] = [shuffledImages[j], shuffledImages[i]];
            }
        }

        function updateKomsais() {
            const targetKomsai = document.getElementById("targetImage");
            const healingKomsai = document.getElementById("healingImage");
            if(targetKomsai && healingKomsai) {
                targetKomsai.src = shuffledImages[1].src; // Type 1 is Visual Target
                healingKomsai.src = shuffledImages[0].src; // Type 0 is Visual Healer
            }
        }

        let lastKomsaiSpawn = 0;
        const SPAWN_COOLDOWN = 1500; 

        let lastBossShoot = 0;
        const BOSS_SHOOT_COOLDOWN = 500;

        let loadedCount = 0;
        komsaiImages.forEach(img => {
            img.onload = () => {
                loadedCount++;
                if (loadedCount === komsaiImages.length) {
                    // [FIXED] Force an initial shuffle/update before loop starts
                    shuffleKomsaiImages();
                    updateKomsais();
                    
                    if(bgMusic) bgMusic.play().catch(e => console.log("Audio pending interaction"));
                    loop(0); 
                }
            }
        });

        const keysPressed = {};
        let lastShotTime = 0;
        const SHOOT_COOLDOWN = 500;

        document.addEventListener("keydown", (event) => {
            keysPressed[event.key] = true;
            if (event.key === "p" || event.key === "P") {
                isPaused = !isPaused;
                const pauseMenu = document.getElementById("pause-menu");
                if (isPaused) {
                    if(pauseMenu) pauseMenu.style.display = 'flex';
                    if(!sfxAlarm.paused) sfxAlarm.pause();
                } else {
                    if(pauseMenu) pauseMenu.style.display = 'none';
                    lastTime = performance.now();
                    requestAnimationFrame(loop); 
                }
            }
        });

        document.addEventListener("keyup", (event) => { keysPressed[event.key] = false; });

        function handleInput() {
            if (keysPressed["ArrowRight"]) right_movement();
            if (keysPressed["ArrowLeft"]) left_movement();

            const now = Date.now();
            if (keysPressed[" "] && now - lastShotTime >= SHOOT_COOLDOWN) {
                shoot_bullet();
                startSound.currentTime = 0;
                startSound.play();
                triggerShake('small');
                lastShotTime = now;
            }
        }

        function spawnKomsai() {
            const now = Date.now();
            if (now - levelStartTime < 3000) return; 

            const currentCooldown = Math.max(200, SPAWN_COOLDOWN - (get_game_level() * 50));
            if (now - lastKomsaiSpawn >= currentCooldown) {
                generate_komsai();
                lastKomsaiSpawn = now;
            }
        }

        function renderShip(currentLife) {
            ctx.save();
            ctx.shadowBlur = 15;
            ctx.shadowColor = "cyan";

            if (currentLife <= 2 && currentLife > 0) {
                ctx.fillStyle = `rgba(255, 0, 0, ${Math.random() * 0.5 + 0.5})`; 
                ctx.shadowColor = "red";
            } else {
                ctx.fillStyle = '#e0ffff';
            }

            const shipX = getX();
            const shipY = canvas.height - 80;
            for (let y = 0; y < shipPattern.length; y++) {
                for (let x = 0; x < shipPattern[y].length; x++) {
                    if (shipPattern[y][x]) ctx.fillRect(shipX + x * scale, shipY + y * scale, scale, scale);
                }
            }
            ctx.restore();
            return { x: shipX, y: shipY };
        }

        function updateLifeBoxes(playerLife) {
            const lifeBoxes = document.querySelectorAll(".lifeContainer .lifeBox");
            lifeBoxes.forEach((box, index) => {
                if (index < playerLife) {
                    box.style.backgroundColor = "#00ff00"; 
                    box.style.boxShadow = "0 0 5px #00ff00";
                } else {
                    box.style.backgroundColor = "rgba(255, 255, 255, 0.1)"; 
                    box.style.boxShadow = "none";
                }
            });
        }

        function getEnemiesList() {
            const count = komsai_count();
            const list = [];
            for (let i = 0; i < count; i++) {
                list.push({
                    x: Module._get_komsai_x(i),
                    y: Module._get_komsai_y(i),
                    type: get_komsai_type(i) 
                });
            }
            return list;
        }

        let lastTime = 0;

        function loop(timestamp) {
            if (isPaused) return;

            if (!lastTime) lastTime = timestamp;
            const dt = (timestamp - lastTime) / 16.6667;
            lastTime = timestamp;
            const safeDt = Math.min(dt, 4.0); 

            ctx.clearRect(0, 0, canvas.width, canvas.height);

            // Instructions Screen Check
            const instructionsScreen = document.getElementById("instructions-screen"); 
            if (instructionsScreen && getComputedStyle(instructionsScreen).display !== 'none') {
                 handleInput();
                 renderShip(5); 
                 ctx.save();
                 ctx.shadowBlur = 10;
                 ctx.shadowColor = "#ffff00"; ctx.strokeStyle = "#ffff00"; ctx.lineWidth = 4; ctx.lineCap = "round";
                 for (let i = 0; i < bullet_count(); i++) {
                     const bx = Module._get_bullet_x(i);
                     const by = Module._get_bullet_y(i);
                     ctx.beginPath(); ctx.moveTo(bx + 4, by); ctx.lineTo(bx + 4, by + 20); ctx.stroke();
                 }
                 ctx.restore();
                 requestAnimationFrame(loop);
                 return; 
            }

            const currentLife = get_player_life();
            const currentScore = get_score();
            const currentLevel = get_game_level() + 1; 

            window.starSpeedMultiplier = 1 + (get_game_level() * 0.3);

            if (comboTimer > 0) {
                comboTimer -= (dt * 16.66);
            } else {
                comboCount = 0;
            }

            if(currentLife > 0 && currentLife <= 2 && sfxAlarm.paused) {
                sfxAlarm.play().catch(e => {});
            }
            
            handleInput();

            const enemiesBefore = getEnemiesList();
            update(safeDt); 
            const enemiesAfter = getEnemiesList();

            // --- LEVEL LOGIC ---
            if (currentLevel !== lastKnownLevel) {
                lastKnownLevel = currentLevel;
                levelStartTime = Date.now(); 
                
                const levelTextEl = document.getElementById("levelText");
                const currentLevelSpan = document.getElementById("currentLevel");
                
                if(currentLevelSpan) currentLevelSpan.innerText = currentLevel;
                if(levelTextEl) {
                    levelTextEl.classList.remove("level-anim");
                    void levelTextEl.offsetWidth; 
                    levelTextEl.classList.add("level-anim");
                }

                if (currentLevel - lastShuffledLevel >= 1) {
                    shuffleKomsaiImages();
                    updateKomsais();
                    lastShuffledLevel = currentLevel;
                }
            }

            // --- SCORE / KILL DETECTION ---
            if (currentScore > previousScore) {
                const diff = currentScore - previousScore; 
                sfxExplosion.currentTime = 0; 
                sfxExplosion.play();

                comboCount++;
                comboTimer = COMBO_MAX_TIME;
                
                let spawnX = 0;
                let spawnY = 0;
                let foundDead = false;

                for(let prev of enemiesBefore) {
                    const stillAlive = enemiesAfter.some(curr => 
                        Math.abs(curr.x - prev.x) < 2 &&  
                        Math.abs(curr.y - prev.y) < 20    
                    );
                    
                    if (!stillAlive) {
                        spawnX = prev.x + 75; 
                        spawnY = prev.y + 75; 
                        foundDead = true;
                        
                        triggerShake('medium');

                        // [FIXED] Strict Color Mapping to Type ID
                        // Type 0 is drawn as shuffledImages[0] -> Visual Healer -> Green
                        // Type 1 is drawn as shuffledImages[1] -> Visual Target -> Red
                        let explosionColor = "orange"; 
                        if (prev.type === 0) {
                            explosionColor = "#00ff00"; 
                        } else if (prev.type === 1) {
                            explosionColor = "red"; 
                        }

                        createExplosion(spawnX, spawnY, explosionColor);

                        break; 
                    }
                }

                if (!foundDead) {
                    spawnX = getX() + 20;
                    spawnY = 150;
                }
                
                const text = comboCount > 1 ? `+${diff} (x${comboCount})` : `+${diff}`;
                const color = comboCount > 4 ? "#ff00ff" : "#ffff00";
                floatingTexts.push(new FloatingText(spawnX, spawnY, text, color));
                previousScore = currentScore;
            }

            // --- HEALTH EVENT DETECTION ---
            const vignette = document.querySelector('.critical-overlay');
            if (currentLife !== previousLife) {
                if (currentLife < previousLife) {
                    triggerShake('medium');
                    sfxDamage.currentTime = 0;
                    sfxDamage.play();
                    const sX = getX();
                    const sY = canvas.height - 80;
                    createExplosion(sX + 20, sY + 20, "red");
                } else if (currentLife > previousLife) {
                    sfxHeal.currentTime = 0;
                    sfxHeal.play();
                }
                previousLife = currentLife;
            }

            if (currentLife <= 2 && currentLife > 0) {
                if(vignette) vignette.classList.add('active');
            } else {
                if(vignette) vignette.classList.remove('active');
            }

            // --- ALARM LOGIC ---
            if (currentLife > 0 && currentLife <= 2) {
                if (sfxAlarm.paused) {
                    sfxAlarm.currentTime = 0; 
                    sfxAlarm.play().catch(e => {});
                }
            } else {
                if (!sfxAlarm.paused) {
                    sfxAlarm.pause();
                    sfxAlarm.currentTime = 0;
                }
            }

            // --- RENDER SHIP ---
            const shipCoords = renderShip(currentLife);
            const shipX = shipCoords.x;
            const shipY = shipCoords.y;

            // --- RENDER PARTICLES (GLOW) ---
            ctx.save();
            ctx.globalCompositeOperation = "lighter"; 
            particles.forEach((p, index) => {
                p.update();
                p.draw(ctx);
                if (p.life <= 0) particles.splice(index, 1);
            });
            ctx.restore();

            // --- RENDER TEXTS ---
            floatingTexts.forEach((ft, index) => {
                ft.update();
                ft.draw(ctx);
                if (ft.life <= 0) floatingTexts.splice(index, 1);
            });

            // --- RENDER COMBO HUD ---
            if (comboCount > 1) {
                ctx.save();
                ctx.fillStyle = "rgba(255, 255, 0, 0.7)";
                ctx.font = "20px 'Pixelify Sans'";
                ctx.textAlign = "right";
                ctx.fillText(`COMBO x${comboCount}`, canvas.width - 20, 100);
                const barWidth = 100 * (comboTimer / COMBO_MAX_TIME);
                ctx.fillStyle = "orange";
                ctx.fillRect(canvas.width - 120, 110, barWidth, 5);
                ctx.restore();
            }

            // --- RENDER BULLETS ---
            ctx.save();
            ctx.shadowBlur = 10;
            ctx.shadowColor = "#ffff00"; 
            ctx.strokeStyle = "#ffff00";
            ctx.lineWidth = 4;
            ctx.lineCap = "round";
            for (let i = 0; i < bullet_count(); i++) {
                const x = Module._get_bullet_x(i);
                const y = Module._get_bullet_y(i);
                ctx.beginPath(); ctx.moveTo(x + 4, y); ctx.lineTo(x + 4, y + 20); ctx.stroke();
            }
            ctx.restore();

            // --- RENDER ENEMIES/BOSS ---
            const internalLevel = get_game_level(); 
            
            if ((internalLevel+1) % 3 == 0 && internalLevel > 0) {
                // BOSS LEVEL
                if (!bossSpawned) {  
                    if (Date.now() - levelStartTime > 3000) {
                        spawnBoss();
                        bossSpawned = true;
                        const bossHud = document.getElementById("boss-hud");
                        if(bossHud) bossHud.style.display = "flex";
                        triggerShake('large'); 
                    }
                }

                if (bossSpawned) {
                    const now = Date.now();
                    if (now - lastBossShoot >= BOSS_SHOOT_COOLDOWN) {
                        shoot_bullet_boss();
                        sfxBossShoot.currentTime = 0;
                        sfxBossShoot.play().catch(e => {});
                        lastBossShoot = now;
                    }

                    const currentBossHP = get_boss_health(0);
                    const hpPercent = Math.max(0, (currentBossHP / 20) * 100);
                    const bossFill = document.getElementById("boss-health-fill");
                    if(bossFill) bossFill.style.width = `${hpPercent}%`;

                    if (currentBossHP > 0) {
                        const x = get_boss_x(0);
                        const y = get_boss_y(0);
                        ctx.drawImage(bossImg, x, y, 200, 200);
                    } else {
                        triggerShake('large');
                        bossSpawned = false;
                        const bossHud = document.getElementById("boss-hud");
                        if(bossHud) bossHud.style.display = "none";
                    }
                }
            } else {
                // NORMAL LEVELS
                bossSpawned = false;
                const bossHud = document.getElementById("boss-hud");
                if(bossHud) bossHud.style.display = "none";

                spawnKomsai();
                for (let i = 0; i < komsai_count(); i++) {
                    const x = Module._get_komsai_x(i);
                    const y = Module._get_komsai_y(i);
                    const type = get_komsai_type(i);
                    const randomImg = shuffledImages[type];
                    ctx.drawImage(randomImg, x, y, 100, 100);
                }
            }

            // --- UI & GAME OVER ---
            if (get_score() > parseInt(sessionStorage.getItem("highScore"))) {
                sessionStorage.setItem("highScore", get_score());
            } else if (!sessionStorage.getItem("highScore")) {
                sessionStorage.setItem("highScore", "0");
            }

            const scoreEl = document.getElementById("scoreValue");
            const highScoreEl = document.getElementById("highScoreValue");
            if(scoreEl) scoreEl.innerText = `${get_score()}`;
            if(highScoreEl) highScoreEl.innerText = `${sessionStorage.getItem("highScore") || 0}`;
            
            updateLifeBoxes(currentLife);

            if (currentLife <= 0 ) {
                if(!sfxAlarm.paused) sfxAlarm.pause();
                if(vignette) vignette.classList.remove('active');

                sfxExplosion.currentTime = 0;
                sfxExplosion.play();
                
                triggerShake('large');

                createExplosion(shipX, shipY, "cyan"); 
                ctx.drawImage(explosionImg, shipX-20, shipY-20, 100, 100);
                setTimeout(() => {
                    const gameOverDiv = document.getElementById("game-over");
                    const gameScreenDiv = document.getElementById("game-screen");
                    if(gameScreenDiv) gameScreenDiv.style.display = 'none';
                    if(gameOverDiv) gameOverDiv.style.display = 'flex';
                }, 3000);
                
                if (typeof loadGameOverScript === "function") {
                    loadGameOverScript();
                }

                if (get_score() > parseInt(sessionStorage.getItem("highScore") || "0")) {
                    if(highScoreEl) highScoreEl.innerText = `${get_score()}`;
                    sessionStorage.setItem("highScore", get_score());
                }
                return;
            }

            requestAnimationFrame(loop);
        }
    }

    initWhenModuleReady();
};