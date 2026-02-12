const express = require('express');
const app = express();

app.get('/', (req, res) => res.send('Hello World!'));

PORT = 3000;
app.listen(PORT, () => console.log(`listen on ${PORT}`));