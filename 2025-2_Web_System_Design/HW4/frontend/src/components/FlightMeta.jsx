export default function FlightMeta({ flight }) {
  return (
    <p id="flightInfo" className="lines">
      {flight
        ? `${flight.code} | ${flight.from}→${flight.to} | ${flight.date} | ${flight.departureTime} → ${flight.arrivalTime}`
        : "항공정보 로드 중..."}
    </p>
  );
}
