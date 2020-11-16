/********************************************************************
 *
 *                       WEBSOCKET SECTION
 *
 ********************************************************************/

var wsUri = "ws://192.168.0.1:3000/";
// Game variables
var gamePlayers;
var gamePenalty;
var state;

function init() {
    output = document.getElementById("output");
    testWebSocket();
}
// Declare Websockets funktions
function testWebSocket() {
    websocket = new WebSocket(wsUri);
    websocket.onopen = function(evt) { onOpen(evt) };
    websocket.onclose = function(evt) { onClose(evt) };
    websocket.onmessage = function(evt) { onMessage(evt) };
    websocket.onerror = function(evt) { onError(evt) };
}
// Websocket connected
function onOpen(evt) {
    writeToScreen("The Game is Online and ready to play..!");
    doSend("User connected from webinterface");
}
// Websocket disconnected
function onClose(evt) {
    writeToScreen("The Game connection is lost, too bad :-(");
}
// check for penalty or AVGtime
function onMessage(evt) {
    writeToScreen('<span style="color: blue;">RAW: ' + evt.data + '</span>');
    if (isJSON(evt.data)) {
        var package = JSON.parse(evt.data);
        switch (package.gameStatus) {
            case 0: // Ready for new game
                // Set Page State
                state = "PlayerNames";
                checkState();
                break;
            case 1: // On going game - create cards and sort
                gamePlayers = []; // Clear Array
                for (var i = 0; i < package.players.length; i++) {
                    gamePlayers[i] = new playerObj(package.players.name, colorIndex[i], i, package.players.penalty, package.players.AVGtime);
                }
                gamePenalty = package.maxPenalty;
                // Set Page State
                state = "gameOn";
                checkState();
                break;
            case 2: // End Game - show the winner
                // Set Page State
                state = "endGame";
                checkState();
                break;
            default:
                break;
        }
    } else {
        writeToScreen('<span style="color: blue;">RESPONSE: ' + evt.data + '</span>');
    }
}
// Error Message to screen
function onError(evt) {
    writeToScreen('<span style="color: red;">Bad day for a Game:</span> ' + evt.data);
}
// Send to websocket
function doSend(message) {
    writeToScreen("SENT: " + message);
    websocket.send(message);
}
// Write to screen function
function writeToScreen(message) {
    var pre = document.createElement("h1"); // Create element
    pre.style.wordWrap = "break-word"; // Wrap text to fit on page
    pre.innerHTML = message; // Pu recieved message in inner HTML
    document.getElementById("output").appendChild(pre); // Apped to "output" on page
    console.log(message);
}

window.addEventListener("load", init, false);

// Check recived me
function isJSON(data) {
    if ((data[0] == '{' && data[data.length - 1] == '}') ||
        (data[0] == '[' && data[data.length - 1] == ']')) {
        return true
    } else {
        return false
    }
}


/********************************************************************
 *
 *                       GAME STATES
 *
 *              Write debug(true/fasle) in console
 *              activate/deactivate output logging
 *
 ********************************************************************/
/*
function stateShift(newState {
    console.log(newState);
    switch (newState) {
        case "PlayerNames":
            localStorage.setItem("state", newState);
            checkState();
            break;
        case "gameOn":
            localStorage.setItem("state", newState);
            checkState();
            break;
        case "endGame":
            localStorage.setItem("state", newState);
            checkState();
            break;
        default:
            break;
    }

}*/

function checkState() {
    switch (state) {
        case "PlayerNames":
            // Hide section gameOn og endGame ************************/
            deleteStorage();
            document.getElementById("gameOn").style.display = "none";
            document.getElementById("endGame").style.display = "none";
            /*********************************************************/
            // Show section PlayerNames ******************************/
            document.getElementById("PlayerNames").style.display = "block";
            /*********************************************************/
            break;
        case "gameOn":
            // Hide section PlayerNames og endGame *******************/
            document.getElementById("PlayerNames").style.display = "none";
            document.getElementById("endGame").style.display = "none";
            /*********************************************************/
            // Show section gameOn ***********************************/
            document.getElementById("gameOn").style.display = "block";
            Cards();
            /*********************************************************/
            break;
        case "endGame":
            // Delete cards *****************************************/
            deletePenaltyList();
            deleteavgTimeList();
            // Hide section PlayerNames og gameOn *******************/
            document.getElementById("PlayerNames").style.display = "none";
            document.getElementById("gameOn").style.display = "none";
            /********************************************************/
            // Show section endGame *********************************/
            document.getElementById("endGame").style.display = "block";
            /********************************************************/
            break;
        default:
            // Hide section gameOn og endGame ************************/
            deleteStorage();
            document.getElementById("gameOn").style.display = "none";
            document.getElementById("endGame").style.display = "none";
            /*********************************************************/
            // Show section PlayerNames ******************************/
            document.getElementById("PlayerNames").style.display = "block";
            /*********************************************************/
            localStorage.setItem("debug", true);
            break;
    }
}

/*******************************************************************
 *
 *                       GAME COMMANDS
 *
 ********************************************************************/
function newGame() {
    // Generator newGame message
    var JSON_newGame = {
        gameStatus: 0
    };
    console.log(JSON.stringify(JSON_newGame));
    doSend(JSON.stringify(JSON_newGame));
}

function startGame() {
    createPlayers();
    // Read Set Penalty
    gamePenalty = Number(document.getElementById("maxPenalty").value);
    // Get PlayerNames
    var input = document.getElementById("playerInput").elements;
    gamePlayers = [];
    for (i = 0; i < input.length; i++) {
        if (input[i].type === "text") {
            gamePlayers[gamePlayers.length] = { name: input[i].value };
        };
    };
    // Generator start message
    var JSON_start = {
        gameStatus: 1,
        maxPenalty: gamePenalty,
        gameMode: 1,
        players: gamePlayers
    };
    console.log(JSON.stringify(JSON_start));
    doSend(JSON.stringify(JSON_start));
}

function stopGame() {
    // Generator stop message
    var JSON_stop = {
        gameStatus: 2,
    };
    console.log(JSON.stringify(JSON_stop));
    doSend(JSON.stringify(JSON_stop));
}

/*
function debug(me) {
    switch (me) {
        case true:
            document.getElementById("output").style.display = "block";
            localStorage.setItem("debug", true);
            break;
        default:
            document.getElementById("output").style.display = "none";
            localStorage.setItem("debug", false);
            break;
    }
}
*/

/*******************************************************************
 *
 *                       PLAYER SETTINGS
 *
 ********************************************************************/

// Color index array, to compare PSoC Input
var colorIndex = ["blue", "brown", "black", "orange", "purple", "red", "green", "yellow"];

// Player Object Class
class playerObj {
    constructor(setName, color, penalty = 0, setAvgTime = 0) {
        this.name = setName; // Name from Start Web Page
        this.color = color; // Color
        this.penalty = penalty; // Penalty Variable
        this.avgTime = setAvgTime; // AVG time Variable
    }
}

// Create localStorage with "playerInput" and "maxPenalty"
function createPlayers() {
    var input = document.getElementById("playerInput").elements;
    var players = [];
    for (i = 0; i < input.length; i++) {
        if (input[i].type === "text") {
            players[players.length] = new playerObj(input[i].value, colorIndex[players.length]);
        }
    }
    
    localStorage.setItem('players', JSON.stringify(players));

    var penalty = document.getElementById("maxPenalty").value;
    localStorage.setItem('penalty', penalty);
}

// Delete localStorage 
function deleteStorage() {
    localStorage.removeItem(playerInput);
    localStorage.removeItem(maxPenalty);
}

/*******************************************************************
 *
 *                       PAGE LAYOUT
 *
 ********************************************************************/

// Create cards from "input"
function Cards() {
    //    var players = JSON.parse(localStorage.getItem('players'));
    //    console.log(gamePlayers);
    createCards(gamePlayers, "Penalty"); // Create card for column Penalty
    createCards(gamePlayers, "avgTime"); // Create card for column AVG time

    var item = document.getElementById("PenaltyHeader");
    item.parentNode.removeChild(item);

    var penaltyHeader = document.createElement('h3');
    penaltyHeader.innerHTML = "Max straf point: " + gamePenalty;
    penaltyHeader.setAttribute("id", "PenaltyHeader");
    document.getElementById("penaltyTitle").appendChild(penaltyHeader);
}

// Create cards "input"/"sort" with column. From a given player to a page location(AVGtime or penalty)
function createCards(players, location) {

    for (i = 0; i < players.length; i++) {
        if ((i + 1) % 2 && i != 0) {
            var newline = document.createElement('div');
            newline.className = 'w-100';
            document.getElementById(location).appendChild(newline);
        }

        if (i <= 3) {
            var column = document.createElement('div');
            column.className = 'col';
            column.appendChild(CardElement(players[i], i));
            document.getElementById(location).appendChild(column);
        } else if (i > 3) {
            var column = document.createElement('div');
            column.className = 'col';
            column.appendChild(CardElement(players[i], i));
            document.getElementById(location).appendChild(column);
        }
    }
}

// Create card elements
function CardElement(player, index) {
    var newCard = document.createElement('div');
    // newCard.className = 'mb-3';
    newCard.classList.add(player.color);
    newCard.setAttribute("style", "width: 10rem");
    // newCard.
    // Header text
    cardHeader = document.createElement('div');
    cardHeader.setAttribute("id", "bootstrapOpacity");
    cardHeader.className = 'card-header';
    // cardHeader
    headertext = document.createElement('h5');
    cardHeader.setAttribute("style", "color: white");
    headertext.innerHTML = player.name;
    cardHeader.appendChild(headertext);
    newCard.appendChild(cardHeader);
    // Body
    var body = document.createElement('small');
    body.className = 'card-body';
    var title = document.createElement('h8');
    title.className = 'card-title';
    title.innerHTML = 'Primary card title';
    body.appendChild(title);
    var textField = document.createElement('p');
    textField.className = 'card-text';

    var statusPenalty = document.createElement('button');
    statusPenalty.className = 'btn btn-danger btn-sm btn-block';
    statusPenalty.setAttribute('id', index);
    statusPenalty.innerHTML = 'Antal straf point: ' + player.penalty;
    statusPenalty.setAttribute("onclick", "updatePenalty(this)");
    textField.appendChild(statusPenalty);

    var avgTime = document.createElement('button');
    avgTime.className = 'btn btn-success btn-sm btn-block';
    avgTime.setAttribute('id', index);
    avgTime.innerHTML = 'Gennemsnits tid: ' + player.avgTime;
    avgTime.setAttribute("onclick", "updateAvgTime(this)");
    textField.appendChild(avgTime);

    body.appendChild(textField);
    newCard.appendChild(body);

    return newCard;
}

/*******************************************************************
 *
 *            UPDATE PENALTY AND AVG TIME
 *
 ********************************************************************/
/*
// increment penalty
function updatePenalty(me) {
    // Search array index
    var index;
    var players = JSON.parse(localStorage.getItem('players'));

    for (i = 0; i < players.length; i++) {
        // if (players[i].color === me.parentElement.parentElement.parentElement.className) {
        if (players[i].color === colorIndex[me.index]) {
            index = i;
        }
    }
    // Increment penalty
    players[index].penalty++;
    // Check if the game is over...!
    if (players[index].penalty == localStorage.getItem('penalty')) {
        window.location.href = "endGame.html";
    }

    localStorage.setItem('players', JSON.stringify(players));
    me.innerHTML = 'Antal straf point: ' + players[index].penalty;
    sortPenalty();
}

// update average time
function updateAvgTime(me) {
    // Search array index
    var index;
    var players = JSON.parse(localStorage.getItem('players'));

    for (i = 0; i < players.length; i++) {
        // if (players[i].color === me.parentElement.parentElement.parentElement.className) {
        if (players[i].color === colorIndex[me.index]) {
            index = i;
        }
    }
    // calc Average time
    let numElements = players[index].meassuredTime.push(me.time / 10); // Push time measssurement from PSoC -> teenth of secounds
    let sum = players[index].meassuredTime.reduce(add = (a, b) => a + b);
    players[index].avgTime = sum / numElements;

    localStorage.setItem('players', JSON.stringify(players));
    me.innerHTML = 'Gennemsnits tid: ' + players[index].avgTime;
    sortAVGtime();
}*/

/*******************************************************************
 *
 *         DELETE PENALTY AND AVG TIME LISTS
 *
 ********************************************************************/

// Delete Penalty List
function deletePenaltyList() {
    var check = document.getElementById('Penalty');
    while (check.firstChild) {
        check.removeChild(check.firstChild);
    }
}

// Delete avgTime List
function deleteavgTimeList() {
    var check = document.getElementById('avgTime');
    while (check.firstChild) {
        check.removeChild(check.firstChild);
    }
}

/*******************************************************************
 *
 *         SORT PENALTY AND AVG TIME LISTS
 *
 ********************************************************************/

// Sort penalty
function sortPenalty() {
    deletePenaltyList();
    // sort cards
    // var players = JSON.parse(localStorage.getItem('players'));
    var penalty = gamePlayers.slice(0);
    penalty.sort(function(a, b) {
        return b.penalty - a.penalty;
    });
    // Create cards
    createCards(penalty, 'Penalty');
}

// Sort AVGtime
function sortAVGtime() {
    deleteavgTimeList();
    // sort cards
    // var players = JSON.parse(localStorage.getItem('players'));
    var avgTime = gamePlayers.slice(0);
    avgTime.sort(function(a, b) {
        return a.avgTime - b.avgTime;
    });
    // Create cards
    createCards(avgTime, 'avgTime');
}