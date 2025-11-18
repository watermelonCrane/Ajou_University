import { useEffect, useState } from "react";
import FlightMeta from "./components/FlightMeta";
import ReservationForm from "./components/ReservationForm";
import ReservationTable from "./components/ReservationTable";
import StatusBar from "./components/StatusBar";

export default function App() {
  const [flight, setFlight] = useState(undefined); //항공 정보
  const [reserv, setReserv] = useState([]); // 예약자 리스트
  const [name, setName] = useState(""); // 폼 입력 이름
  const [seat, setSeat] = useState(""); // 폼 입력 좌석
  const [deleteQ, setDeleteQ] = useState([]); // 삭제 예정 예약 큐
  const [status, setStatus] = useState(undefined); // 상태바 상태
  const [msg, setMsg] = useState(""); // 상태바 메시지
  const [seatInfo, setSeatInfo] = useState(undefined); // 예약 가능 좌석 정보 저장

  const checkInput = () => {
    // 좌석 형식에 맞게 다시 작성 ex) 01A -> 1A, 범위 검사
    const row = String(parseInt(seat.slice(0, -1), 10));
    const col = seat.slice(-1).toUpperCase();

    if (name.lenght < 2) makeStatusBar("alert", "이름을 2자 이상 입력하세요");
    else if (!/^\d{1,9}[A-Z]$/.test(String(seat).toUpperCase().trim()))
      makeStatusBar("alert", "좌석 형식이 올바르지 않습니다.");
    else if (row <= 0 || row > flight.rows || !flight.cols.includes(col))
      makeStatusBar(
        "alert",
        `좌석 범위를 벗어낫습니다. 유효 범위: 행 1-${
          flight.rows
        } 열 ${flight.cols.join(", ")}`
      );
    else {
      return true;
    }

    return false;
  };

  // 상태바 생성 함수
  const makeStatusBar = (state, message) => {
    setStatus(state);
    setMsg(message);

    setTimeout(() => {
      setStatus(undefined);
      setMsg("");
    }, 3000);
  };

  // 공통 fetch 유틸
  const fetchAPI = async (URL, { method = "GET", body } = {}) => {
    const option = {
      method,
      headers: { ...(body && { "Content-Type": "application/json" }) },
      body,
    };

    const data = await fetch(URL, option);
    if (!data.ok) throw { status: data.status, message: await data.text() };

    // console.log(data);

    return await data.json();
  };

  // 폼 제출 버튼 핸들
  const handleSubmit = async (e) => {
    e.preventDefault();
    const data = { name, seat };
    if (!checkInput()) return;
    try {
      const res = await fetchAPI("/api/reservations", {
        method: "POST",
        body: JSON.stringify(data),
      });
      setName("");
      setSeat(""); // 폼 초기화

      makeStatusBar("success", `예약 완료: ${res.name} / ${res.seat}`); // 상태바 생성
      getReserv(); // 예약자 새로고침
    } catch (error) {
      // console.log(error || "예약 실패");
      makeStatusBar("alert", error.message); // 상태바 생성
    }
  };

  // 예약 삭제
  const deleteData = async (id) => {
    try {
      const res = await fetchAPI(`/api/reservations/${id}`, {
        method: "DELETE",
      });
      makeStatusBar("success", `삭제 완료: ${res.name} / ${res.seat}`); // 상태바 생성
      getReserv();
    } catch (error) {
      // console.log(error || "삭제 실패");
      makeStatusBar("alert", error.message); // 상태바 생성
    }
  };

  // 예약 삭제 버튼 핸들
  const handleDelete = (id) => {
    const timer = setTimeout(() => {
      deleteData(id);
      setDeleteQ((prev) => prev.filter((d) => d.id !== id));
    }, 3000);

    const data = { id: id, timer: timer, count: 3 };

    setDeleteQ((prev) => [...prev, data]);

    setTimeout(() => {
      setDeleteQ((prev) =>
        prev.map((item) =>
          item.id === id ? { ...item, count: item.count - 1 } : item
        )
      );
      setTimeout(() => {
        setDeleteQ((prev) =>
          prev.map((item) =>
            item.id === id ? { ...item, count: item.count - 1 } : item
          )
        );
        setTimeout(() => {
          setDeleteQ((prev) =>
            prev.map((item) =>
              item.id === id ? { ...item, count: item.count - 1 } : item
            )
          );
        });
      }, 1000);
    }, 1000);
  };

  // 삭제 취소 버튼 핸들
  const handleCancelDelete = (id) => {
    const found = deleteQ.find((d) => d.id === id);

    clearTimeout(found.timer);
    setDeleteQ((prev) => prev.filter((d) => d.id !== id));

    makeStatusBar("info", `삭제 취소됨 (#${found.id})`); // 상태바 생성
  };

  // 항공편 정보 get
  const getFlight = async () => {
    try {
      const res = await fetchAPI("/api/flight");
      setFlight(res);
      setSeatInfo(
        `입력 가능 범위: 행 1-${res.rows} / 열 ${res.cols.join(", ")}`
      );
    } catch (error) {
      console.log(error.message || "비행편 정보 로드 실패");
      makeStatusBar("alert", "비행편 정보 로드 실패"); // 상태바 생성
    }
  };

  // 예약자 정보 get
  const getReserv = async () => {
    try {
      const res = (await fetchAPI("/api/reservations")).slice(1);
      setReserv(res);
    } catch (error) {
      console.log(error.message || "예약 목록 로드 실패");
      makeStatusBar("alert", "예약 목록 로드 실패"); // 상태바 생성
    }
  };

  // 마운트시 항공편, 예약자 정보 get
  useEffect(() => {
    const initData = async () => {
      getFlight();
      getReserv();
    };

    initData();
  }, []);

  return (
    <>
      <header>
        <h1>비행기 좌석 예약</h1>
      </header>
      <article className="contents">
        <div className="menu">
          <FlightMeta flight={flight} />
          <div className="form-outer">
            <ReservationForm
              name={name}
              setName={setName}
              seat={seat}
              setSeat={setSeat}
              buttonHandle={handleSubmit}
              seatInfo={seatInfo}
            />
            <StatusBar status={status} message={msg} />
          </div>
          <div className="form-outer">
            <ReservationTable
              value={reserv}
              deleteQueue={deleteQ}
              deleteHandle={handleDelete}
              cancelHandle={handleCancelDelete}
            />
          </div>
        </div>
      </article>
    </>
  );
}
