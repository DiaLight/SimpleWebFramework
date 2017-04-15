
function main(address) {
    let margin = {top: 5, right: 40, bottom: 20, left: 120},
        width = 960 - margin.left - margin.right,
        height = 50 - margin.top - margin.bottom;

    let chart = d3.bullet()
        .width(width)
        .height(height);

    d3.json("bullet/bullet.json", function(error, data) {
        if (error) throw error;

        let svg = d3.select("body").selectAll("svg")
            .data(data)
            .enter().append("svg")
            .attr("class", "bullet")
            .attr("width", width + margin.left + margin.right)
            .attr("height", height + margin.top + margin.bottom)
            .append("g")
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")")
            .call(chart);

        let title = svg.append("g")
            .style("text-anchor", "end")
            .attr("transform", "translate(-6," + height / 2 + ")");

        title.append("text")
            .attr("class", "title")
            .text(function(d) { return d.title; });

        title.append("text")
            .attr("class", "subtitle")
            .attr("dy", "1em")
            .text(function(d) { return d.subtitle; });

        d3.selectAll("button").on("click", function() {
            svg.datum(randomize).call(chart.duration(1000)); // TODO automatic transition
        });


        let socket = new WebSocket(address);

        socket.onopen = function() {
            console.log('Соединение установлено.');
        };

        socket.onclose = function(event) {
            if (event.wasClean) {
                console.log('Соединение закрыто чисто. Код: ' + event.code + ' причина: ' + event.reason);
            } else {
                console.log('Обрыв соединения. Код: ' + event.code + ' причина: ' + event.reason);
            }
        };

        socket.onmessage = function(event) {
            let object = JSON.parse(event.data);
            svg.datum(function (data) {
                data.markers[0] = object.val;
                return data;
            }).call(chart.duration(200));
        };

        socket.onerror = function(error) {
            console.log('Ошибка ' + error.message);
        };
    });
}

function randomize(d) {
    if (!d.randomizer) d.randomizer = randomizer(d);
    d.ranges = d.ranges.map(d.randomizer);
    d.markers = d.markers.map(d.randomizer);
    d.measures = d.measures.map(d.randomizer);
    return d;
}

function randomizer(d) {
    let k = d3.max(d.ranges) * .2;
    return function(d) {
        return Math.max(0, d + k * (Math.random() - .5));
    };
}