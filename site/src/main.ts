import ApexCharts from "apexcharts";
import { initializeApp } from "firebase/app";
import {
  DocumentData,
  QuerySnapshot,
  collection,
  getFirestore,
  limitToLast,
  onSnapshot,
  orderBy,
  query,
} from "firebase/firestore";
import { firebaseConfig } from "./firebase-config";

import ptBR from "apexcharts/dist/locales/pt-br.json";

import "./style.css";

interface SensorData {
  value: number;
}

const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

let series: ApexAxisChartSeries = [
  {
    name: "Turbidez",
    data: [],
  },
  {
    name: "Condutividade",
    data: [],
  },
];

const options = {
  chart: {
    locales: [ptBR],
    defaultLocale: "pt-br",
    type: "area",
    width: "100%",
    height: 400,
    animations: {
      initialAnimation: {
        enabled: false,
      },
    },
  },
  series: series,
  xaxis: {
    type: "number",
  },
};

const chart = new ApexCharts(
  document.querySelector('[data-js="chart"]'),
  options
);

chart.render();

const turbidityQuery = query(
  collection(db, "turbidity"),
  orderBy("time"),
  limitToLast(20)
);
const unsubscribeTurbidity = onSnapshot(turbidityQuery, (snapshot) => {
  const docs: number[] = [];

  snapshot.forEach((doc) => {
    const docData = doc.data() as SensorData;
    docs.push(docData.value);
  });

  series = [{ name: "Turbidez", data: docs }, series[1]];

  chart.updateSeries(series, false);
});

const conductivityQuery = query(
  collection(db, "conductivity"),
  orderBy("time"),
  limitToLast(20)
);
const unsubscribeConductivity = onSnapshot(conductivityQuery, (snapshot) => {
  const docs: number[] = [];

  snapshot.forEach((doc) => {
    const docData = doc.data() as SensorData;
    docs.push(docData.value);
  });

  series = [series[0], { name: "Condutividade", data: docs }];

  chart.updateSeries(series, false);
});

addEventListener("beforeunload", () => {
  unsubscribeTurbidity();
  unsubscribeConductivity();
});
