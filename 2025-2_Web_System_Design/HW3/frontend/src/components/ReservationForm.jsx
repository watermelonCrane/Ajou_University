export default function ReservationForm({
  name,
  seat,
  setName,
  setSeat,
  buttonHandle,
  seatInfo,
}) {
  return (
    <>
      <h2>좌석 예약</h2>
      <div className="form-inner">
        <form>
          <label htmlFor="name">탑승객 이름</label>
          <input
            value={name}
            onChange={(e) => setName(e.target.value)}
            type="text"
            name="name"
            id="name"
            placeholder="홍길동"
          />
          <p>2글자 이상 입력하세요.</p>
          <label htmlFor="seat">좌석</label>
          <input
            value={seat}
            onChange={(e) => setSeat(e.target.value)}
            type="text"
            name="seat"
            id="seat"
            placeholder="3C"
          />
          <p className="lines">
            예약할 좌석을 입력하세요. 형식: 행번호 + 열(예:7B)
            <br />
            {seatInfo ?? ""}
          </p>
          <button onClick={buttonHandle}>예약하기</button>
        </form>
      </div>
    </>
  );
}
