const fs         = require( "fs" )
const bodyParser = require( "body-parser" );
const express    = require( "express" )
const readline   = require( "readline" )

const ALARMS_FILE = __dirname + "/../alarms.dat"
const UPDATE_FILE = "/tmp/updateAlarmsKooreminder.dat"

var app = express()
app.use( bodyParser.urlencoded({ extended: false }) );
app.use( bodyParser.json() );

// Functions
var CreateUpdateFile = function() {
    fs.writeFile( UPDATE_FILE, "", (error) => {
        if ( error ) {
            console.log( '[ERROR] CreateUpdateFile: cannot create update file' )
            console.log( '[ INFO] error ->', error )
        }
    } )
}

var AddAlarm = function( time, repeat, title ) {
    var newTitle = title.replace( / /g, '_' )
    fs.appendFile( ALARMS_FILE, time + " " + repeat + " " + newTitle + "\n", (error) => {
        if ( error ) {
            console.log( '[ERROR] AddAlarm: cannot append to file' )
            console.log( '[ INFO] error ->', error )
        } else {
            CreateUpdateFile()
        }
    } )
}

var RemoveAlarm = function( alarmId ) {
    let rl = readline.createInterface( {
        input: fs.createReadStream( ALARMS_FILE )
    } );

    let lineNumber = 0;
    let buffer = ""

    rl.on( 'line', function(line) {
        if ( lineNumber != alarmId ) {
            buffer += line + "\n"
        }

        lineNumber++
    } );
    
    rl.on( 'close', function(line) {
        fs.writeFile( ALARMS_FILE, buffer, (error) => {
            if ( error ) {
                console.log( '[ERROR] RemoveAlarm: cannot overwrite alarms file' )
            }
        })
    } );

    CreateUpdateFile()
}

// Server Routing
app.get( '/', ( request, response ) => {
    response.send( fs.readFileSync( 'index.html', 'utf8' ) )
    console.log( 'sent:', fs.readFileSync( 'index.html', 'utf8' ) )
} )

app.get( '/addAlarm', ( request, response ) => {
    console.log( '[ERROR] called /addAlarm with a GET request!' )
} )

app.get( '/removeAlarm', ( request, response ) => {
    console.log( '[ERROR] called /removeAlarm with a GET request!' )
} )

app.get( /.*/, ( request, response) => {
    console.log( '[ERROR] invalid path requested:', request.url )
} )

app.post( '/addAlarm', ( request, response ) => {
    console.log( 'Adding an alarm...' )
    console.log( 'Parsing POST data...' )
    console.log( 'request body:', request.body )
    console.log( 'alarm time:', request.body.time )
    console.log( 'alarm repeat:', request.body.repeat )
    console.log( 'alarm title:', request.body.title )
    AddAlarm( request.body.time, request.body.repeat, request.body.title )
    response.send( {hello: 'world-added alarm!'} )
} )

app.post( '/removeAlarm', ( request, response ) => {
    console.log( 'Removing an alarm...' )
    console.log( 'Parsing POST data...' )
    console.log( 'request body:', request.body )
    console.log( 'alarm id:', request.body.alarm_id )
    RemoveAlarm( request.body.alarm_id )
    response.send( {hello: 'world-removed alarm!'})
} )

// Create the server
var server = app.listen( 8080, () => {
    var host = server.address().address
    var port = server.address().port

    console.log( "Listening on http://%s:%s", host, port )
} )
