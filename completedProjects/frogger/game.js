var sprite	  = new Image();
sprite.src	  = "assets/frogger_sprites.png";
var deathAnim = new Image();
deathAnim.src = "assets/deathSprites.png";
var deathIcon = new Image();
deathIcon.src = "assets/dead_frog.png";

var gameInfo;
var frogLog; //used to hold the current log frogger is on
var cars	= new Array();
var logs	= new Array();
var lilies	= new Array();
var INITIALTIME		= 90;
var LEFTROADSTART	= -30;
var RIGHTROADSTART	= 399;
var LEFTLOGSTART	= -86;
var RIGHTLOGSTART	= 399;
var NUMROWS 		= 5;

function startGame(){
	canvas	= document.getElementById("game");
	ctx		= canvas.getContext("2d");
	initializeObjects();
	drawBoard();
	addListeners();

	gameInterval = setInterval(gameLoop, 30);
	timerID		 = setTimeout(decrementTime, 1000);
}

function gameLoop(){
	if (gameInfo.lives != 0){
		updateObjects();
		drawBoard();
	}
	else{
		alert ("Game Over... Refresh the page to play again!");
		clearInterval(gameInterval);
	}
}

function initializeObjects(){
	gameInfo= new Info();
	frogger = new Frog();
	gameTime = INITIALTIME;
	for (i = 0; i < NUMROWS; i++){
		lilies[i] = new LilyPad(i);
	}
	createLogsAndCars();
}

/* -----------------------------------------------------------------------------------
		BOARD DRAWING FUNCTIONS
   ----------------------------------------------------------------------------------- */

function drawBoard(){
	ctx.fillStyle = "#191970";
	ctx.fillRect(0, 0, 399, 275);
	ctx.fillStyle = "#000000";
	ctx.fillRect(0, 275, 399, 565);

	addSlice(15, 10, 320, 35, 10, 10,  320, 35);
	addSlice(0,  55, 399, 55,  0, 50,  399, 55);
	addSlice(0, 118, 399, 36,  0, 275, 399, 40);
	addSlice(0, 118, 399, 36,  0, 485, 399, 40);

	ctx.fillStyle = "#135920";

	for (i in lilies){
		ctx.fillRect(lilies[i].x, lilies[i].y, lilies[i].width, lilies[i].height);
		if (lilies[i].hasFrog){
			addSlice(78, 368, 24, 18, lilies[i].x - 1, lilies[i].y + 2, 24, 18);
		}
	}	
	level 		  = "Level: " + gameInfo.level;
	ctx.font      = "bold 20px Arial";
	ctx.fillStyle = "#00FF00";	
	ctx.fillText(level, 150, 540);

	var time   = "Time: " + gameTime;
	score 	   = "Score: " + gameInfo.score;
	ctx.font   = "bold 15px Arial";	
	ctx.fillText(score, 2, 560);
	ctx.fillText(time, 275, 540);

	var lives = "Lives: ";
	ctx.fillText(lives, 2, 538);
	for (i = 0; i < gameInfo.lives; i++){
		addSlice(11, 334, 20, 22, gameInfo.xLives, 525, 15, 16);
		gameInfo.xLives += 15;
	}
	gameInfo.xLives = 45;

	for (i in cars){
		cars[i].draw();
	}
	for (i in logs){
		logs[i].draw();
	}
	frogger.draw();
	collisionTest();
}

function updateObjects(){
	for (i in cars){
		if (cars[i].onLeft){
			cars[i].coordinates.x += cars[i].speed;
			if (cars[i].coordinates.x >= cars[i].end){
				cars[i].coordinates.x = LEFTROADSTART;
			}
		}
		else{
			cars[i].coordinates.x -= cars[i].speed;
			if (cars[i].coordinates.x <= cars[i].end){
				cars[i].coordinates.x = RIGHTROADSTART;
			}
		}
	}
	for (i in logs){
		if (logs[i].onLeft){
			logs[i].coordinates.x += logs[i].speed;
			if (logs[i].hasFrog){
				frogger.coordinates.x += frogger.speed;
				if (frogger.coordinates.x > 375){
					froggerDead();
				}
			}
			if (logs[i].coordinates.x >= logs[i].end){
				logs[i].coordinates.x = LEFTLOGSTART;
			}
		}
		else{
			logs[i].coordinates.x -= logs[i].speed;
			if (logs[i].hasFrog){
				frogger.coordinates.x -= frogger.speed;
				if (frogger.coordinates.x < 0){
					froggerDead();
				}
			}
			if (logs[i].coordinates.x <= logs[i].end){
				logs[i].coordinates.x = RIGHTLOGSTART;
			}
		}
	}
	if ((gameInfo.score / 10000 != gameInfo.livesAdded) && (gameInfo.score % 10000 == 0)){
		gameInfo.lives++;
		gameInfo.livesAdded++;
	}
	var counter = 0;
	for (i in lilies){
		if (lilies[i].hasFrog){
			counter++;
		}
	}
	if (counter == 5){
		levelUp();
	}
}

//only used for drawing static images, objects have the addImage method.
function addSlice(sx, sy, sWidth, sHeight, x, y, width, height){
	ctx.drawImage(sprite, sx, sy, sWidth, sHeight, x, y, width, height);
}

function addImage(){
	var coordinates = this.coordinates;

	ctx.drawImage(sprite, coordinates.sx, coordinates.sy, coordinates.width, 
	coordinates.height,   coordinates.x,  coordinates.y,  coordinates.xSize, 
	coordinates.ySize);
}

/* -----------------------------------------------------------------------------------
		FROGGER MOVEMENT FUNCTIONS
   ----------------------------------------------------------------------------------- */

function addListeners(){
	moveInProgress = false;
	document.addEventListener("keydown", function(event){
		if (gameInfo.lives == 0){
			this.removeEventListener("keydown", arguments.callee, false);
			return;
		}
	    if ((event.keyCode == 37) && (!moveInProgress)){
			move("left");
		}
		else if ((event.keyCode == 38) && (!moveInProgress)){
			move("up");
		}
		else if ((event.keyCode == 39) && (!moveInProgress)){
			move("right");
		}
		else if ((event.keyCode == 40) && (!moveInProgress) && (frogger.coordinates.y < 492)){
			move("down");
		}
  	});
}

function move(direction){
	frogDirection  = direction;
	moveInProgress = true;
	currentX	   = frogger.coordinates.x;
	currentY	   = frogger.coordinates.y;

	if (direction == "left"){
		frogger.coordinates = new Coordinates(110, 337, 25, 22, currentX, currentY, 25, 22);
		frogger.isMoving = true;
		if (frogger.onLog){
			frogger.onLog	= false;
			frogLog.hasFrog	= false;
		}
		counter = 0		
		intervalID = setInterval(moveFrog, 30);
	}
	else if (direction == "right"){
		frogger.coordinates = new Coordinates(42, 335, 25, 22, currentX, currentY, 25, 24);
		frogger.isMoving = true;
		if (frogger.onLog){
			frogger.onLog	= false;
			frogLog.hasFrog	= false;
		}
		counter = 0;
		intervalID = setInterval(moveFrog, 30);
	}
	else if (direction == "up"){
		frogger.coordinates = new Coordinates(43, 365, 24, 25, currentX, currentY, 24, 25);
		frogger.isMoving = true;
		if (frogger.onLog){
			frogger.onLog	= false;
			frogLog.hasFrog	= false;
		}
		counter = 0;
		intervalID = setInterval(moveFrog, 30);
	}
	else{
		frogger.coordinates = new Coordinates(112, 365, 24, 25, currentX, currentY, 24, 25);
		frogger.isMoving = true;
		if (frogger.onLog){
			frogger.onLog	= false;
			frogLog.hasFrog	= false;
		}
		counter = 0;
		intervalID = setInterval(moveFrog, 30);
	}
}

function moveFrog(){
	if (frogDirection == "left"){
		frogger.coordinates.x -= frogger.horzSpeed;
		currentX -= frogger.horzSpeed;
		counter += frogger.horzSpeed;
		drawBoard();		
		if (counter == 25){
			frogger.coordinates = new Coordinates (80, 335, 18, 24, currentX, currentY, 18, 24);
			if (currentX < 0){
				froggerDead("explode");
			}
			frogger.isMoving = false;	
			moveInProgress	 = false;		
			clearInterval(intervalID);
		}	
	}
	else if (frogDirection == "right"){
		frogger.coordinates.x += frogger.horzSpeed;
		currentX += frogger.horzSpeed;
		counter	 += frogger.horzSpeed;
		drawBoard();
		if (counter == 25){
			frogger.coordinates = new Coordinates (11, 334, 18, 24, currentX, currentY, 18, 24);
			if (currentX > 399){
				froggerDead("explode");
			}
			frogger.isMoving = false;
			moveInProgress	 = false;				
			clearInterval(intervalID);
		}	
	}
	else if (frogDirection == "up"){
		frogger.coordinates.y -= frogger.vertSpeed;
		currentY -= frogger.vertSpeed;
		counter  += frogger.vertSpeed;
		drawBoard();		
		if (counter == 34){
			frogger.coordinates = new Coordinates (10, 368, 24, 18, currentX, currentY, 24, 18);
			frogger.isMoving = false;
			moveInProgress	 = false;
			gameInfo.score += 10;
			clearInterval(intervalID);
		}			
	}
	else{
		frogger.coordinates.y += frogger.vertSpeed;
		currentY += frogger.vertSpeed;
		counter	 += frogger.vertSpeed;
		drawBoard();		
		if (counter == 34){
			frogger.coordinates = new Coordinates (78, 368, 24, 18, currentX, currentY, 24, 18);
			frogger.isMoving = false;
			moveInProgress	 = false;
			clearInterval(intervalID);
		}			
	}		
}

/* -----------------------------------------------------------------------------------
		COLLISION TESTING FUNCTIONS
   ----------------------------------------------------------------------------------- */

function collisionTest(){
	frogBoxWidth	= frogger.coordinates.width;
	frogBoxHeight	= frogger.coordinates.height;

	if (frogger.coordinates.y >= 315){
		for (i in cars){
			testCarCollision(cars[i]);
		}
	}
	else{
		for (i in lilies){
			testLilypadCollision(lilies[i]);
		}
		if (!frogger.isMoving){
			for (i in logs){
				testLogCollision(logs[i]);
			}
			if (!frogger.onLog){
				testWaterCollision();
			}
		}
	}
}

function testCarCollision(car){
	var carX		  = car.coordinates.x;
	var carY		  = car.coordinates.y;
	var carWidth	  = car.coordinates.width;
	var carHeight	  = car.coordinates.height;

	for (i = carX; i < carX + carWidth; i++){
		if ((i >= frogger.coordinates.x) && (i <= frogger.coordinates.x + frogBoxWidth)){
			for (k = carY; k < carY + carHeight; k++){
				if ((k >= frogger.coordinates.y) && (k <= frogger.coordinates.y + frogBoxHeight)){
					froggerDead("explode");
					return;
				}
			}
		}
	}
}

function testLilypadCollision(lily){
	for (i = lily.x; i < lily.x + lily.width; i++){
		if ((i >= frogger.coordinates.x) && (i <= frogger.coordinates.x + frogBoxWidth)){
			for (k = lily.y; k < lily.y + lily.height; k++){
				if ((k >= frogger.coordinates.y) && (k <= frogger.coordinates.y + frogBoxHeight)){
					if (lily.hasFrog){
						froggerDead("explode");
						return;
					}
					else{
						froggerHome(lily);
						return;
					}
				}
			}
		}
	}
}

function testLogCollision(log){
	var logX		  = log.coordinates.x;
	var logY		  = log.coordinates.y;
	var logWidth	  = log.coordinates.width;
	var logHeight	  = log.coordinates.height;

	for (i = logX; i < logX + logWidth; i++){
		if ((i >= frogger.coordinates.x) && (i <= frogger.coordinates.x + frogBoxWidth)){
			for (k = logY; k < logY + logHeight; k++){
				if ((k >= frogger.coordinates.y) && (k <= frogger.coordinates.y + frogBoxHeight)){
					froggerOnLog(log);
					return;
				}
			}
		}
	}
}

function testWaterCollision(){
	var waterX	= 0;
	var waterY	= 50;
	var wWidth	= 400;
	var wHeight	= 225;

	for (i = waterX; i < waterX + wWidth; i++){
		if ((i >= frogger.coordinates.x) && (i <= frogger.coordinates.x + frogBoxWidth)){
			for (k = waterY; k < waterY + wHeight; k++){
				if ((k >= frogger.coordinates.y) && (k <= frogger.coordinates.y + frogBoxHeight)){
					froggerDead("drown");
					return;
				}
			}
		}
	}
}

function froggerOnLog(log){
	log.hasFrog		= true;
	frogLog			= log;
	frogger.onLog	= true;
	frogger.speed	= log.speed;
}

function froggerHome(lily){
	lily.hasFrog = true;
	clearInterval(intervalID);
	moveInProgress = false;
	gameInfo.score += 60; //50 for getting home + 10 for moving forward
	gameInfo.score += (gameTime * 10);
	gameTime = INITIALTIME;
	frogger = new Frog();
}

function froggerDead(deathType){
	clearInterval(gameInterval);
	clearTimeout(timerID);
	gameInfo.lives--;
	if (moveInProgress){
		clearInterval(intervalID);
		moveInProgress = false;
	}
	animateDeath(deathType);
	gameTime = INITIALTIME;
}

/* -----------------------------------------------------------------------------------
		ADDITIONAL REQUIREMENT FUNCTIONS
   ----------------------------------------------------------------------------------- */
function animateDeath(deathType){
	animX = frogger.coordinates.x;
	if (animX > 399){
		animX = 375;
	}
	else if (animX < 0){
		animX = 0;
	}	
	animY = frogger.coordinates.y;

	//moves frogger off the canvas for the death animation
	frogger.coordinates = new Coordinates (1, 1, 1, 1, -100, -100, 1, 1);
	drawBoard();

	animation			= new Object();
	animation.draw		= drawDeath;
	animation.imageNum	= 0; //used to tell the draw function which image to draw	
	animation.images	= new Array();
	animation.source	= deathAnim;

	if (deathType == "explode"){
		for (i = 0; i < 4; i++){
			if (i == 0){
				animation.images[i] = new Coordinates(90, 6, 14, 15, animX, animY, 14, 15);
			}
			else if (i == 1){
				animation.images[i] = new Coordinates(113, 6, 17, 15, animX, animY, 17, 15);
			}
			else if (i == 2){
				animation.images[i] = new Coordinates(136, 5, 18, 17, animX, animY, 18, 17);
			}
			else if (i == 3){
				animation.images[i] = new Coordinates(3, 2, 21, 26, animX, animY, 21, 26);
			}
		}
	}
	else{
		for (i = 0; i < 4; i++){
			if (i == 0){
				animation.images[i] = new Coordinates(10, 7, 15, 13, animX, animY, 15, 13);
			}
			else if (i == 1){
				animation.images[i] = new Coordinates(34, 5, 15, 16, animX, animY, 15, 16);
			}
			else if (i == 2){
				animation.images[i] = new Coordinates(57, 5, 17, 17, animX, animY, 17, 17);
			}
			else if (i == 3){
				animation.images[i] = new Coordinates(3, 2, 21, 26, animX, animY, 21, 26);
			}
		}
	}
	setTimeout(animation.draw, 300);
}

function drawDeath(){
	drawBoard();
	if (animation.imageNum == 4){
		animation.imageNum = 0;
		animation.source = deathAnim;
		frogger = new Frog();
		gameInterval = setInterval(gameLoop, 30);
		timerID		 = setTimeout(decrementTime, 1000);
		return;
	}

	var num			= animation.imageNum;
	var coordinates = animation.images[num];

	ctx.drawImage(animation.source, coordinates.sx, coordinates.sy, coordinates.width, 
	coordinates.height,   coordinates.x,  coordinates.y,  coordinates.xSize, 
	coordinates.ySize);

	if (animation.imageNum == 2){
		animation.source = deathIcon;
	}
	animation.imageNum++;
	setTimeout(animation.draw, 300);
}

function decrementTime(){
	if (gameTime != 0){
		gameTime--;
		timerID = setTimeout(decrementTime, 1000);
	}
	else{
		froggerDead("explode");
		gameTime = INITIALTIME;
		timerID = setTimeout(decrementTime, 1000);
	}
}

function levelUp(){
	gameInfo.level++;
	for (i in lilies){
		lilies[i].hasFrog = false;
	}
	for (i in cars){
		cars[i].speed += .5;
	}
	for (i in logs){
		logs[i].speed += .5;
	}
	INITIALTIME -= 5;
	gameTime 	 = INITIALTIME;
}

/* -----------------------------------------------------------------------------------
		OBJECT CREATION FUNCTIONS
   ----------------------------------------------------------------------------------- */

function Info(){
	this.score      = 0;
	this.lives      = 3;
	this.livesAdded = 0;
	this.level      = 1;
	this.highscore  = 0;
	this.xLives     = 45; // The x coordinate to place the life image on the canvas
	this.time	    = 0;
	this.gameOver   = false;
}

function Frog(){
	this.coordinates = new Coordinates(10, 368, 24, 18, 175, 492, 24, 18);
	this.horzSpeed	 = 5;  //used when frogger moves side to side
	this.vertSpeed	 = 8.5;//used when frogger moves up or down
	this.speed		 = 0;  //used when frogger is on a log
	this.onLog		 = false;
	this.isMoving	 = false;
	this.hasLadyFrog = false;
	this.ateFly		 = false;
	this.draw		 = addImage;
}

function LilyPad(col){
	this.x		 = 15 + (85 * col);
	this.y		 = 75;
	this.width	 = 24;
	this.height	 = 24;
	this.hasFrog = false;
}

//this function creates all the cars and logs that are used in the game.  There are
//3 cars per row and 2 logs per row. 
function createLogsAndCars(){
	var row  = 0;
	for (i = 0; i < 9; i++){
		cars[i] = new Car(row, ((i + 1) % 3), false);
		if ((i != 0) && ((i + 1) % 3 == 0)){
			row += 2;
		}
	}
	row = 1;
	for (i = 9; i < 15; i++){
		cars[i] = new Car(row, ((i + 1) % 3), true);
		if ((i != 9) && ((i + 1) % 3 == 0)){
			row += 2;
		}
	}
	row = 0;

	for (i = 0; i < 6; i++){
		logs[i] = new Log(row, (i % 2), false);
		if ((i != 0) && ((i + 1) % 2 == 0)){
			row += 2;
		}
	}
	row = 1;
	for (i = 6; i < 10; i++){
		logs[i] = new Log(row, (i % 2), true);
		if ((i != 6) && ((i + 1) % 2 == 0)){
			row += 2;
		}
	}
}

function Car(row, objNum, onLeft){
	yStart = 320 + (row * 34); //road is also 400 x 170

	if (row == 0){
		this.xStart	= 399 - (110 * objNum); 
		this.end	= -30;
		this.speed	= 1.5;
		this.coordinates = new Coordinates(8, 267, 30, 20, this.xStart, yStart, 46, 34);
	}
	else if (row == 1){
		this.xStart	= -30 + (140 * objNum);
		this.end	= 429;
		this.speed	= 2;
		this.coordinates = new Coordinates(8, 299, 26, 24, this.xStart, yStart, 40, 34);
	}	
	else if (row == 2){
		this.xStart	= 399 - (175 * objNum); 
		this.end	= -30;
		this. speed	= 1.25;
		this.coordinates = new Coordinates(82, 264, 30, 25, this.xStart, yStart, 42, 34);
	}
	else if (row == 3){
		this.xStart	= -30 + (55 * objNum);
		this.end	= 429;
		this.speed	= 3;
		this.coordinates = new Coordinates(45, 264, 30, 25, this.xStart, yStart, 42, 34);
	}
	else if (row == 4){
		this.xStart	= 399 - (125 * objNum); 
		this.end	= -48;
		this.speed	= 1.5;
		this.coordinates = new Coordinates(104, 302, 48, 20, this.xStart, yStart, 64, 34);
	}
	this.onLeft	= onLeft;
	this.draw	= addImage;
}

function Log(row, objNum, onLeft){
	yStart = 110 + (row * 34); //water is 400 x 170, board is split into 5 rows this way

	if (row == 0){
		this.xStart	= 399 - (250 * objNum + 30); 
		this.end	= -118;
		this.speed  = 2;
		this.coordinates = new Coordinates(5, 197, 118, 24, this.xStart, yStart, 118, 34);
	}
	else if (row == 1){
		this.xStart	= -86 + (175 * objNum + 40);
		this.end	= 399;
		this.speed	= 1.25;
		this.coordinates = new Coordinates(5, 229, 86, 24, this.xStart, yStart, 86, 34);
	}	
	else if (row == 2){
		this.xStart	= 399 - (140 * objNum); 
		this.end	= -118;
		this.speed	= 1.5;
		this.coordinates = new Coordinates(5, 197, 118, 24, this.xStart, yStart, 118, 34);
	}
	else if (row == 3){
		this.xStart	= -86 + (175 * objNum);
		this.end	= 399;
		this.speed	= 3;
		this.coordinates = new Coordinates(5, 229, 86, 24, this.xStart, yStart, 86, 34);
	}
	else if (row == 4){
		this.xStart	= 399 - (200 * objNum); 
		this.end	= -180;
		this.speed	= 1;
		this.coordinates = new Coordinates(5, 165, 180, 24, this.xStart, yStart, 180, 34);
	}

	this.onLeft	 = onLeft;
	this.hasFrog = false;
	this.draw	 = addImage;
}

//contains all information needed for drawing the image of the object
function Coordinates(sx, sy, sWidth, sHeight, x, y, xSize, ySize, xEnd){
	this.sx		= sx;      // starting x-coordinate for slicing
	this.sy		= sy;      // starting y-coordinate for slicing
	this.width	= sWidth;  // width of the image slice
	this.height	= sHeight; // height of the image slice
	this.x		= x;       // x-location on the board
	this.y		= y;       // y-location on the board
	this.xSize	= xSize;   // width of the frog image
	this.ySize	= ySize;   // height of the frog image
}