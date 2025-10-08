const express = require('express');
const app = express();
const path = require('path');

const PORT = process.env.PORT || 3000;

// app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname)));



// 항공편 예시 데이터 객체
const flight = {
    id: "F124",
    code: "AJ101",
    date: "2025-10-03",
    departureTime: "10:30",
    arrivalTime: "11:40",
    from: "PAL",
    to: "HYK",
    rows: 12,
    cols: ["A", "B", "C", "D", "E", "F"],
};

// 예약목록 배열
const reservations = [
    { id: 1, name: "홍길동", seat: "1A", ts: "2025-09-24T16:18:33.719Z" },
];




//좌석 형식 검사 함수
function seatPatternOK(seat) {
    return /^\d{1,9}[A-Z]$/.test(String(seat).toUpperCase().trim());
}

//좌석 범위 검사 함수
function seatRangeOK(seat) {
    const row = parseInt(seat.slice(0, -1), 10);
    const col = seat.slice(-1);

    //console.log('in', row);

    if (row <= 0 || row > flight.rows) {     //row의 범위가 벗어난다면
        //console.log('detect!');
        return false;
    }
    if (!flight.cols.includes(col)) {       //col의 범위가 벗어난다면
        return false;
    }

    return true;
}

//좌석 예약여부 검사 함수
function seatReservOK(seat) {
    if (reservations.find((e) => e.seat === seat) === undefined) return true;
    else return false;
}



// 라우팅
app.get('/api/flight', (req, res) => {
    res.json(flight);
});

app.route('/api/reservations')
    .get((req, res) => {
        res.json(reservations);
    })
    .post((req, res, next) => {

        let {name, seat} = req.body;

        name = name.trim(); //앞뒤 공백제거
        
        // 좌석 형식에 맞게 다시 작성 ex) 01A -> 1A
        const seatRow = String(parseInt(seat.slice(0, -1), 10));
        const seatCol = seat.slice(-1);
        seat = seatRow + seatCol;


        try {
            if (name.length < 2) throw { status: 400, message: "400 Bad Request, 이름을 더 길게 해주세요." };    //길이 체크
            if (!seatPatternOK(seat)) throw { status: 400, message: "400 Bad Request, 좌석 형식을 확인해주세요." };  //좌석 형식 체크
            if (!seatRangeOK(seat)) throw { status: 400, message: "400 Bad Request, 가능한 좌석 범위를 초과했습니다." }; // 좌석 범위 체크
            if (!seatReservOK(seat)) throw { status: 409, message: "409 Conflict, 이미 예약된 좌석입니다." };    // 빈좌석 체크
        } catch (err) {
            return next(err);
        }

        reservations.push({ id: reservations.length + 1, name: name, seat: seat, ts: new Date().toISOString() })


        res.status(201).json(reservations.at(reservations.length - 1));


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