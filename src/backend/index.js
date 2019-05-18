const fs      = require("fs")
const express = require("express")

var app = express()

app.get( '/', ( request, response ) => {
    response.send( fs.readFileSync( 'index.html', 'utf8' ) )
    console.log( 'sent: ', fs.readFileSync( 'index.html', 'utf8' ) )
} )

app.get( '/addAlarm', ( request, response ) => {
    console.log( 'to implement (add alarm)' )
} )

app.get( '/removeAlarm', ( request, response ) => {
    console.log( 'to implement (remove alarm)' )
} )

var server = app.listen( 8080, () => {
    var host = server.address().address
    var port = server.address().port

    console.log( "Listening on http://%s:%s", host, port )
} )
