window.startGame = function () {
    const canvas = document.getElementById("gameCanvas");
    const ctx = canvas.getContext("2d");

    function resizeCanvas() {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    resizeCanvas();
    window.addEventListener("resize", resizeCanvas);

    function initWhenModuleReady() {
        if (!Module || !Module.calledRun) {
            setTimeout(initWhenModuleReady, 50);
            return;
        }

        const update = Module.cwrap("update", null, []);
        const getX = Module.cwrap("get_x", "number", []);
        const right_movement = Module.cwrap("right_movement", null, []);
        const left_movement = Module.cwrap("left_movement", null, []);
        const shoot_bullet = Module.cwrap("shoot_bullet", null, []);
        const bullet_count = Module.cwrap("get_bullet_count", "number", []);
        const komsai_count = Module.cwrap("get_komsai_count", "number", []);
        const generate_komsai = Module.cwrap("generate_komsai", null, []);
        const get_komsai_type = Module.cwrap("get_komsai_type", "number", ["number"]);
        const get_score = Module.cwrap("get_score", "number", []);
        const get_player_life = Module.cwrap("get_player_life", "number", []);

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

        const startSound = new Audio("assets/music/shoot_sound.mp3");
        startSound.volume = 1;
        let soundPlayed = false;

        // Load Komsai images
        const explosionImg = new Image();
        explosionImg.src = "assets/images/explosion.png";
        const images = [
            "assets/images/Chakie.png",
            "assets/images/Hew2.png"
        ];
        const komsaiImages = images.map(src => {
            const img = new Image();
            img.src = src;
            return img;
        });

        let lastKomsaiSpawn = 0;
        const SPAWN_COOLDOWN = 1000;

        // Wait until all images are loaded
        let loadedCount = 0;
        komsaiImages.forEach(img => {
            img.onload = () => {
                loadedCount++;
                if (loadedCount === komsaiImages.length) loop(); // start loop when all loaded
            }
        });

        const keysPressed = {};
        let lastShotTime = 0;
        const SHOOT_COOLDOWN = 300;

        document.addEventListener("keydown", (event) => { keysPressed[event.key] = true; });
        document.addEventListener("keyup", (event) => { keysPressed[event.key] = false; });

        function handleInput() {
            if (keysPressed["ArrowRight"]) right_movement();
            if (keysPressed["ArrowLeft"]) left_movement();

            const now = Date.now();
            if (keysPressed[" "] && now - lastShotTime >= SHOOT_COOLDOWN) {
                shoot_bullet();
                startSound.currentTime = 0;
                startSound.play()
                lastShotTime = now;
            }
        }

        function spawnKomsai(image_path) {
            const now = Date.now();
            if (now - lastKomsaiSpawn >= SPAWN_COOLDOWN) {
                generate_komsai(image_path);
                lastKomsaiSpawn = now;
            }
        }

        function updateLifeBoxes(playerLife) {
            const lifeBoxes = document.querySelectorAll(".lifeContainer .lifeBox");
            lifeBoxes.forEach((box, index) => {
                if (index < playerLife) {
                    box.style.backgroundColor = "green";   // or any "alive" color
                } else {
                    box.style.backgroundColor = "red";  // "lost life" color
                }
            });
        }

        function loop() {
            handleInput();
            update();
            ctx.clearRect(0, 0, canvas.width, canvas.height);

            // Ship rendering
            ctx.fillStyle = 'white';
            const shipX = getX();
            const shipY = canvas.height - 80;
            for (let y = 0; y < shipPattern.length; y++) {
                for (let x = 0; x < shipPattern[y].length; x++) {
                    if (shipPattern[y][x]) ctx.fillRect(shipX + x * scale, shipY + y * scale, scale, scale);
                }
            }

            // Bullet rendering
            for (let i = 0; i < bullet_count(); i++) {
                const x = Module._get_bullet_x(i);
                const y = Module._get_bullet_y(i);
                ctx.fillRect(x, y, 8, 16);
            }

            // Komsai rendering with random image
            spawnKomsai();
            for (let i = 0; i < komsai_count(); i++) {
                const x = Module._get_komsai_x(i);
                const y = Module._get_komsai_y(i);

                const type = get_komsai_type(i);
                const randomImg = komsaiImages[type];
                ctx.drawImage(randomImg, x, y, 150, 150);
            }

            if (get_score() > parseInt(sessionStorage.getItem("highScore"))) {
                sessionStorage.setItem("highScore", get_score());
            }
            else if (!sessionStorage.getItem("highScore")) {
                sessionStorage.setItem("highScore", "0");
            }

            document.getElementById("scoreValue").innerText = `${get_score()}`;
            document.getElementById("highScoreValue").innerText = `${sessionStorage.getItem("highScore") || 0}`;
            // Update life boxes
            const currentLife = get_player_life();
            updateLifeBoxes(currentLife);

            if (currentLife <= 0 ) {
                ctx.drawImage(explosionImg, shipX-20, shipY-20, 100, 100);
                setTimeout(() => {
                    const gameOverDiv = document.getElementById("game-over");
                    const gameScreenDiv = document.getElementById("game-screen");
                    gameScreenDiv.style.display = 'none';
                    gameOverDiv.style.display = 'flex';
                }, 3000);
                loadGameOverScript();

                if (get_score() > parseInt(sessionStorage.getItem("highScore") || "0")) {
                    document.getElementById("highScoreValue").innerText = `${get_score()}`;
                    sessionStorage.setItem("highScore", get_score());
                }
                return;
            }

            requestAnimationFrame(loop);
        }
    }

    initWhenModuleReady();
};