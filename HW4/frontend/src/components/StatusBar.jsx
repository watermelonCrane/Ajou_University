import "../App.css";

export default function StatusBar({ status, message }) {
  if (status === undefined) {
    return <></>;
  }

  return <div className={status}>{message}</div>;
}
