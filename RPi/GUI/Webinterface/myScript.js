var wsUri = "ws://192.168.0.1:3000/";
var output;
var counter = 0;

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
    writeToScreen('<span style="color: blue;">RESPONSE: ' + evt.data + '</span>');
    // websocket.close();
}

// Error Message to screen
function onError(evt) {
    writeToScreen('<span style="color: red;">Bad day for a Game:</span> ' + evt.data);
}

function doSend(message) {
    writeToScreen("SENT: " + message);
    websocket.send(message);
}

// Write to screen function
function writeToScreen(message) {
    var pre = document.createElement("h1"); // Create element
    pre.style.wordWrap = "break-word"; // Wrap text to fit on page
    pre.innerHTML = message; // Pu recieved message in inner HTML
    output.appendChild(pre); // Apped to "output" on page
}

window.addEventListener("load", init, false);

function clicker() {
    counter = counter + 1;
    doSend("Hello with count: " + counter);
}

function playerObj(input, color, index) {
    this.index = index; // integer to compare with UART input
    this.name = input; // Name from Start Web Page
    this.color = color; // Color
    this.penalty = 0; // Penalty Variable
    this.avgTime = 0; // AVG time Variable
}

function createPlayers() {

    var input = document.getElementById("names").elements;
    var players = [];
    for (i = 0; i < input.length; i++) {
        if (input[i].type === "text") {
            players[players.length] = new playerObj(input[i].value, input[i].id, i);
        }
    }
    localStorage.setItem('players', JSON.stringify(players));

    var penalty = document.getElementById("maxPenalty");
    localStorage.setItem('penalty', penalty.value);

    startGame();
}

function startGame() {

    var JSON_start = {
        "gameCommand" : "start",
        "penalty" : localStorage.getItem('penalty')
    }

    doSend(JSON.stringify(JSON_start));
}

function stopGame() {

    var JSON_stop = {
        "gameCommand" : "stop"
    }

    doSend(JSON.stringify(JSON_stop));
}


// Create cards from input
function Cards() {
    var players = JSON.parse(localStorage.getItem('players'));
    console.log(players);
    createCards(players, "Penalty"); // Create card for column Penalty
    createCards(players, "avgTime"); // Create card for column AVG time

    var penaltyHeader = document.getElementById("penaltyTitle");
    penaltyHeader.innerHTML = "Max straf point: " + localStorage.getItem('penalty');
    document.getElementById("penaltyTitle").appendChild(penaltyHeader);
}

// Create cards input/sort with column 
// location penalty column or AVGtime column
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

// increment penalty
// Sort cards - title, røvhul, vise røvhul, president, vise president, tilfældig type
function updatePenalty(me) {
    // Search array index
    var index;
    var players = JSON.parse(localStorage.getItem('players'));

    for (i = 0; i < players.length; i++) {
        if (players[i].color === me.parentElement.parentElement.parentElement.className) {
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
        if (players[i].color === me.parentElement.parentElement.parentElement.className) {
            index = i;
        }
    }
    // Increment Average time
    players[index].avgTime++;
    localStorage.setItem('players', JSON.stringify(players));
    me.innerHTML = 'Gennemsnits tid: ' + players[index].avgTime;
    sortAVGtime();
}
// Sort penalty
function sortPenalty() {
    // Delete cards
    var check = document.getElementById('Penalty');
    while (check.firstChild) {
        check.removeChild(check.firstChild);
    }
    // sort cards
    var players = JSON.parse(localStorage.getItem('players'));
    var penalty = players.slice(0);
    penalty.sort(function(a, b) {
        return b.penalty - a.penalty;
    });
    // Create cards
    createCards(penalty, 'Penalty');
}

// Sort AVGtime
function sortAVGtime() {
    // Delete cards
    var check = document.getElementById('avgTime');
    while (check.firstChild) {
        check.removeChild(check.firstChild);
    }
    // sort cards
    var players = JSON.parse(localStorage.getItem('players'));
    var avgTime = players.slice(0);
    avgTime.sort(function(a, b) {
        return a.avgTime - b.avgTime;
    });
    // Create cards
    createCards(avgTime, 'avgTime');
}

function deleteStorage() {
    localStorage.clear();
}