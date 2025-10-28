const express = require('express');
const path = require('path');
const fs = require("fs/promises");

const app = express();
const PORT = process.env.PORT || 3000;

// 미들웨어 설정
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname)));

// 파일 경로 설정
const DATA_DIR = path.join(__dirname, "data");
const FLIGHT_FILE = path.join(DATA_DIR, "flight.json");
const RES_FILE = path.join(DATA_DIR, "reservations.json");


// === readJSON Constrol ===

// getJSON
async function loadJSON(filePath) {
    console.log("in load:", filePath);
    const raw = await fs.readFile(filePath, "utf-8");
    console.log("after load");
    return JSON.parse(raw);
}
// saveJSON
async function saveJSON(filePath, data) {
    const json = JSON.stringify(data, null, 4);
    await fs.writeFile(filePath, json, "utf-8");
}


// === read file ===

// flight 정보 읽기
async function readFlight() {
    const flight = await loadJSON(FLIGHT_FILE);
    return flight;
}

// reservations 정보 읽기
async function readReserv() {
    console.log("in read");
    const reserv = await loadJSON(RES_FILE);
    return reserv;
}

// reservations 추가하기
async function updateReserv(newData) {
    const reserv = await loadJSON(RES_FILE);
    const nextId = reserv[0].nextId;
    reserv[0].nextId++;

    const newReserv = { id: nextId, ...newData };
    reserv.push(newReserv);
    await saveJSON(RES_FILE, reserv);

    // return reserv;
}

// reservations 삭제하기
async function deleteReserv(id) {
    const reserv = await loadJSON(RES_FILE);

    console.log(typeof reserv, typeof id);

    const target = reserv.find(data => data.id === parseInt(id));

    console.log(target);

    const modified = reserv.filter(data => data.id !== parseInt(id));
    if (modified.length === reserv.length) throw new Error(`${id} is not found`);

    await saveJSON(RES_FILE, modified);
    return target;
}


// === 좌석 검사 ===

//좌석 형식 검사 함수
function seatPatternOK(seat) {
    return /^\d{1,9}[A-Z]$/.test(String(seat).toUpperCase().trim());
}

//좌석 범위 검사 함수
async function seatRangeOK(seat) {
    const row = parseInt(seat.slice(0, -1), 10);
    const col = seat.slice(-1);

    const flight = await readFlight();    // flight 파일 읽어오기

    console.log('in', row);

    if (row <= 0 || row > flight.rows) {     //row의 범위가 벗어난다면
        //console.log('detect!');
        return false;
    }
    if (!flight.cols.includes(col)) {       //col의 범위가 벗어난다면
        return false;
    }

    console.log("done range");
    return true;
}

//좌석 예약여부 검사 함수
async function seatReservOK(seat) {
    const reserv = await readReserv();    // reservations 파일 읽어오기

    if (reserv.find((e) => e.seat === seat) === undefined) return true;
    else return false;
}


// === 라우팅 ===

app.get('/api/flight', async (req, res) => {
    const flight = await readFlight();
    res.json(flight);
});

app.get('/api/reservations', async (req, res) => {
    console.log("in get");
    const reserv = await readReserv();
    res.json(reserv);
});

app.post('/api/reservations', async (req, res, next) => {
    let { name, seat } = req.body;
    name = name.trim(); //앞뒤 공백제거

    console.log(req.body);

    try {
        // 좌석 형식 검사
        if (name.length < 2) throw { status: 400, message: "400 Bad Request, 이름을 더 길게 해주세요." };    //길이 체크
        if (!seatPatternOK(seat)) throw { status: 400, message: "400 Bad Request, 좌석 형식을 확인해주세요." };  //좌석 형식 체크

        // 좌석 형식에 맞게 다시 작성 ex) 01A -> 1A
        const seatRow = String(parseInt(seat.slice(0, -1), 10));
        const seatCol = seat.slice(-1).toUpperCase();
        seat = seatRow + seatCol;

        // 좌석 범위 체크, 중복 체크
        if (!await seatRangeOK(seat)) throw { status: 400, message: "400 Bad Request, 가능한 좌석 범위를 초과했습니다." }; // 좌석 범위 체크
        if (!await seatReservOK(seat)) throw { status: 409, message: "409 Conflict, 이미 예약된 좌석입니다." };    // 빈좌석 체크
    } catch (err) {
        return next(err);
    }

    console.log("검사 완료");

    // 좌석 예약
    const newReserv = { name: name, seat: seat, ts: new Date().toISOString() };
    await updateReserv(newReserv);

    return res.status(201).json(newReserv);     //예약한 좌석 정보 반환

});

app.delete('/api/reservations/:id', (req, res) => {

    const id = req.params.id;

    console.log("in delete:", id);
    deleteReserv(id)
        .then((result) => {
            res.status(200).json(result);
        })
        .catch((err) => {
            res.status(404).send("해당 예약이 없습니다.");
        });
});


//에러 핸들러
app.use((err, req, res, next) => {
    //console.dir(err);

    res.status(err.status).send(`<h1>${err.message}</h1>`);

})


//서버 실행
app.listen(PORT, () => {
    console.log(`server open on ${PORT}`);
});