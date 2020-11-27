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
// Message from Main program
function onMessage(evt) {
    writeToScreen('<span style="color: red;">RAW: ' + evt.data + '</span>');
    if (isJSON(evt.data)) // Check for JSON Package or text
    {
        var package = JSON.parse(evt.data);
        switch (package.gameStatus) // Check Game status
        {
            case 0: // Start screen
                state = "PlayerNames";
                checkState();
                break;
            case 1: // On going game - create cards and sort
                gamePlayers = []; // Clear Array
                for (var i = 0; i < package.players.length; i++) {
                    gamePlayers[i] = new playerObj(package.players[i].name, colorIndex[i], package.players[i].penalty, package.players[i].avgTime);
                }
                gamePenalty = package.maxPenalty;
                writeToScreen(gamePlayers);
                state = "gameOn";
                checkState();
                break;
            case 2: // Game over - show the winner
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
 *              activate/deactivate page sections
 *
 ********************************************************************/

function checkState() {
    switch (state) {
        case "PlayerNames":
            // Hide section gameOn og endGame ************************/
            gamePlayers = [];
            document.getElementById("gameOn").style.display = "none";
            document.getElementById("endGame").style.display = "none";
            /*********************************************************/
            // Show section PlayerNames ******************************/
            document.getElementById("PlayerNames").style.display = "block";
            /*********************************************************/
            var topbar = document.getElementById('TopBarText');
            topbar.innerHTML = 'Indtast spillernes navne:';
            break;
        case "gameOn":
            // Hide section PlayerNames og endGame *******************/
            document.getElementById("PlayerNames").style.display = "none";
            document.getElementById("endGame").style.display = "none";
            /*********************************************************/
            // Show section gameOn ***********************************/
            document.getElementById("gameOn").style.display = "block";
            /*********************************************************/
            updatePenaltyHeader();
            var topbar = document.getElementById('TopBarText');
            topbar.innerHTML = 'Spillerne er:';
            sortAVGtime();
            sortPenalty();
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
            var topbar = document.getElementById('TopBarText');
            topbar.innerHTML = 'Spillet er slut';
            break;
        default:
            // Hide section gameOn og endGame ************************/
            document.getElementById("gameOn").style.display = "none";
            document.getElementById("endGame").style.display = "none";
            /*********************************************************/
            // Show section PlayerNames ******************************/
            document.getElementById("PlayerNames").style.display = "block";
            /******************* no debug ****************************/
            document.getElementById("output").style.display = "none";
            /*********************************************************/
            break;
    }
}

/*******************************************************************
 *
 *                       GAME INTERFACE COMMANDS
 *
 ********************************************************************/

function newGame() // Shift to Start screen
{
    // Generate message for Main program
    var JSON_newGame = {
        gameStatus: 0
    };
    doSend(JSON.stringify(JSON_newGame));
    // Change page state
    state = "PlayerNames";
    checkState();
}

function startGame() // Shift to gameOn - and Update
{
    createPlayers(); // From index.html
    // Change page state
    state = "gameOn";
    checkState();
    // Read Set Player inputs
    var input = document.getElementById("playerInput").elements;
    gamePlayers = [];
    for (i = 0; i < input.length; i++) {
        if (input[i].type === "text") {
            gamePlayers[gamePlayers.length] = { name: input[i].value };
        };
    };
    // Generate start message
    var JSON_start = {
        gameStatus: 1,
        maxPenalty: parseInt(gamePenalty, 10),
        gameMode: 1,
        players: gamePlayers
    };
    console.log(JSON.stringify(JSON_start));
    doSend(JSON.stringify(JSON_start));
    deletePenaltyList();
    deleteavgTimeList();
    createCards(gamePlayers, 'Penalty');
    createCards(gamePlayers, 'avgTime');
}

function stopGame() // Shift to endGame - show winner
{
    // Generator stop message
    var JSON_stop = {
        gameStatus: 2,
    };
    console.log(JSON.stringify(JSON_stop));
    doSend(JSON.stringify(JSON_stop));
    // Change page state
    state = "endGame";
    checkState();
    sortAVGtime();
    sortPenalty();
}

function debug() // Shift to endGame - show winner
{
    if (document.getElementById("output").style.display == "none") {
        document.getElementById("output").style.display = "block";
    } else {
        document.getElementById("output").style.display = "none";
    }
}

/*******************************************************************
 *
 *                       PLAYER SETTINGS
 *
 ********************************************************************/

// Color index array, to compare PSoC Input
var colorIndex = ["blue", "brown", "black", "orange", "white", "red", "green", "yellow"];
var rankings = ["Chief Engineer", "Technician", "Mechanic", "Designer", "Student", "Random Person", "waterboy", "Homeless Guy"];

// Player Object Class
class playerObj {
    constructor(setName, color, penalty = 0, setAvgTime = 0) {
        this.name = setName; // Name from Start Web Page
        this.color = color; // Color
        this.penalty = penalty; // Penalty Variable
        this.avgTime = setAvgTime; // AVG time Variable
    }
}

function createPlayers() {
    var input = document.getElementById("playerInput").elements;
    var counter = 0;
    gamePlayers = [];
    for (i = 0; i < input.length; i++) {
        if (input[i].type === "text") {
            gamePlayers[counter] = new playerObj(input[i].value, colorIndex[counter], 0, 0);
        }
    }
    gamePenalty = document.getElementById("maxPenalty").value;
    // Create Cards
    createCards(gamePlayers, "Penalty"); // Create card for column Penalty
    createCards(gamePlayers, "avgTime"); // Create card for column AVG time
    updatePenaltyHeader();
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
    newCard.classList.add(colorIndex[index]);
    newCard.setAttribute("style", "width: 10rem");
    newCard.setAttribute("style", "border: solid; border-color: black");
    // newCard.
    // Header text
    cardHeader = document.createElement('div');
    cardHeader.setAttribute("id", "bootstrapOpacity");
    cardHeader.className = 'card-header';
    // cardHeader
    headertext = document.createElement('h5');
    //cardHeader.setAttribute("style", "color: white");
    headertext.innerHTML = player.name;
    cardHeader.appendChild(headertext);
    newCard.appendChild(cardHeader);
    // Body
    var body = document.createElement('small');
    body.className = 'card-body';
    var title = document.createElement('h3');
    title.className = 'card-title';
    title.innerHTML += rankings[index];
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
    if (state == "gameOn") {
        createCards(penalty, 'Penalty');
    } else if (state == "endGame") {
        var looserName = document.getElementById('looser');
        looserName.innerHTML = penalty[0].name;
    }
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
    if (state == "gameOn") {
        createCards(avgTime, 'avgTime');
    } else if (state == "endGame") {
        var looserName = document.getElementById('podium');
        looserName.innerHTML = avgTime[0].name;
    }
}

function updatePenaltyHeader() {
    // Update Penalty header
    var item = document.getElementById("PenaltyHeader");
    item.parentNode.removeChild(item);
    var penaltyHeader = document.createElement('h3');
    penaltyHeader.innerHTML = `Max straf point: ${gamePenalty}`;
    penaltyHeader.setAttribute("id", "PenaltyHeader");
    document.getElementById("penaltyTitle").appendChild(penaltyHeader);
}