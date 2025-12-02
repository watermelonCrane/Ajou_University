export default function ReservationTable({
  value,
  deleteQueue,
  deleteHandle,
  cancelHandle,
}) {
  return (
    <>
      <h2>예약 목록</h2>
      <table>
        <thead>
          <tr>
            <th>ID</th>
            <th>이름</th>
            <th>좌석</th>
            <th>시간</th>
            <th>작업</th>
          </tr>
        </thead>
        <tbody>
          {value.length > 0 ? (
            value.map((data) => {
              const found = deleteQueue.find((d) => d.id === data.id);

              return (
                <tr key={data.id}>
                  <td>{`#${data.id}`}</td>
                  <td>{data.name}</td>
                  <td>{data.seat}</td>
                  <td>{new Date(data.ts).toLocaleString()}</td>
                  <td>
                    {found ? (
                      <button
                        onClick={() => {
                          cancelHandle(data.id);
                        }}
                      >
                        {`${found.count}s 후 영구 삭제`}
                      </button>
                    ) : (
                      <button
                        onClick={() => {
                          deleteHandle(data.id);
                        }}
                      >
                        삭제
                      </button>
                    )}
                  </td>
                </tr>
              );
            })
          ) : (
            <tr>
              <td>예약이 없습니다.</td>
            </tr>
          )}
        </tbody>
      </table>
    </>
  );
}
