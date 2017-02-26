/**
 * Created by dialight on 24.02.17.
 */

function main(socket) {
    let series = new TimeSeries();

    let chart = new SmoothieChart({
        grid: { strokeStyle:'rgb(125, 0, 0)', fillStyle:'rgb(60, 0, 0)',
            lineWidth: 1, millisPerLine: 250, verticalSections: 6, },
        labels: { fillStyle:'rgb(60, 0, 0)' }
    });
    chart.addTimeSeries(series, { strokeStyle:'rgb(0, 255, 0)', fillStyle:'rgba(0, 255, 0, 0.4)', lineWidth:3 });
    chart.delay = 100;
    chart.canvas = document.getElementById("chart");
    setInterval(function() {
        chart.render(chart.canvas);
    }, 30);
    function fitChart() {
        if (chart.options.enableDpiScaling && window && window.devicePixelRatio !== 1) {
            let canvasWidth = window.innerWidth; //chart.canvas.getAttribute('width');
            let canvasHeight = window.innerHeight; //chart.canvas.getAttribute('height');
            chart.canvas.style.width = canvasWidth + 'px';
            chart.canvas.style.height = canvasHeight + 'px';

            chart.canvas.setAttribute('width', canvasWidth * window.devicePixelRatio);
            chart.canvas.setAttribute('height', canvasHeight * window.devicePixelRatio);
            chart.canvas.getContext('2d').scale(window.devicePixelRatio, window.devicePixelRatio);
        }
    }
    fitChart();
    window.addEventListener("resize", fitChart);
    function toggleFullScreen() {
        let doc = window.document;
        let docEl = doc.documentElement;

        let requestFullScreen = docEl.requestFullscreen || docEl.mozRequestFullScreen || docEl.webkitRequestFullScreen || docEl.msRequestFullscreen;
        let cancelFullScreen = doc.exitFullscreen || doc.mozCancelFullScreen || doc.webkitExitFullscreen || doc.msExitFullscreen;

        if(!doc.fullscreenElement && !doc.mozFullScreenElement && !doc.webkitFullscreenElement && !doc.msFullscreenElement) {
            requestFullScreen.call(docEl);
        } else {
            cancelFullScreen.call(doc);
        }
    }
    chart.canvas.addEventListener("mousedown", toggleFullScreen, false);

    let status = document.getElementById("status");

    socket.binaryType = 'arraybuffer';
    socket.onopen = function() {
        status.innerHTML = "Соединение установлено.";
    };

    socket.onclose = function(event) {
        if (event.wasClean) {
            status.innerHTML = 'Соединение закрыто чисто. Код: ' + event.code + ' причина: ' + event.reason;
        } else {
            status.innerHTML = 'Обрыв соединения. Код: ' + event.code + ' причина: ' + event.reason;
        }
    };

    socket.onmessage = function(event) {
        let dis = new DataInputStream(event.data);
        let val = dis.readInt() / 65535.0;
        let time = new Date().getTime();
        series.append(time, val);
    };

    socket.onerror = function(error) {
        status.innerHTML = ("Ошибка " + error.message);
    };
}
