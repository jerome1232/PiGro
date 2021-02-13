// Read data
var data = d3.json("data/sensor_data.json");
const button = d3.selectAll('input[name="units"]');

draw_temp_graph(data, button.property("value"));
draw_light_graph(data);
draw_humidity_graph(data);

button.on('change', function(d) {
  d3.select("#temp_graph").select("svg").remove();
  var isTrue = (this.value === 'true');
  draw_temp_graph(data, isTrue);
})

function draw_temp_graph(data, isFarenheit) {
  data.then(function(data) {
    data.forEach(function(d) {
      d.date = new Date(d.time_stamp * 1000);
      if (isFarenheit) {
        d.t_temp = d.temp * (9.0/5.0) + 32;
      } else {
        d.t_temp = d.temp;
      }
    })
  // set dimensions and margins
  var margin = {top: 50, right: 30, bottom: 70, left: 60 },
    width = 460 - margin.left - margin.right,
    height = 400 - margin.top - margin.bottom;

  // Create svg
  var svg = d3.select("#temp_graph")
    .append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
    .append("g")
      .attr("transform",
            "translate(" + margin.left + "," + margin.top + ")");

  // Add Graph title
  svg.append("text")
    .attr("x", (width / 2 ))
    .attr("y", 0 - (margin.top / 2))
    .attr("text-anchor", "middle")
    .style("font-size", "1.4em")
    .style("text-decoration", "underline")
    .text("Temperature");

  // Add X axis
  var x = d3.scaleTime()
    .domain(d3.extent(data, function(d) { return d.date; }))
    .range([0, width]);
  svg.append("g")
    .attr("transform", "translate(0," + height + ")")
    .call(d3.axisBottom(x))
    .selectAll("text")
        .style("text-anchor", "end")
        .attr("dx", "-.8em")
        .attr("dy", ".15em")
        .attr("transform", "rotate(-65)");

  // Add X axis label
  svg.append("text")
    .attr("transform",
          "translate(" + (width/2) + " ," +
                         (height + margin.top + 40) + ") ")
    .style("text-anchor", "middle")
    .text("Time");


  // Add Y axis
  minY = d3.min(data, function(d) { return +d.t_temp } );
  maxY = d3.max(data, function(d) { return +d.t_temp } );

  var y = d3.scaleLinear()
    .domain([minY, maxY])
    .range([height, 0]);
  svg.append("g")
    .call(d3.axisLeft(y));

  // Add Y axis label
  svg.append("text")
    .attr("transform", "rotate(-90)")
    .attr("y", 0 - margin.left)
    .attr("x", 0 - (height / 2))
    .attr("dy", "1em")
    .style("text-anchor", "middle")
    .text("Degrees");

  // Add a line
  svg.append("path")
    .datum(data)
    .attr("fill", "none")
    .attr("stroke", "brown")
    .attr("stroke-width", 1.5)
    .attr("d", d3.line()
      .x(function(d) { return x(d.date) })
      .y(function(d) { return y(d.t_temp) })
    )
  })
}

function draw_light_graph(data) {
  data.then(function(data) {
    // set dimensions and margins
    var margin = {top: 50, right: 30, bottom: 70, left: 60 },
      width = 460 - margin.left - margin.right,
      height = 460 - margin.top - margin.bottom;

    // Create svg
    var svg = d3.select("#light_graph")
      .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
      .append("g")
        .attr("transform",
              "translate(" + margin.left + "," + margin.top + ")");

    // Add Graph title
    svg.append("text")
      .attr("x", (width / 2 ))
      .attr("y", 0 - (margin.top / 2))
      .attr("text-anchor", "middle")
      .style("font-size", "1.4em")
      .style("text-decoration", "underline")
      .text("Light level");

    // Add X axis
    var x = d3.scaleTime()
      .domain(d3.extent(data, function(d) { return d.date; }))
      .range([0, width]);
    svg.append("g")
      .attr("transform", "translate(0," + height + ")")
      .call(d3.axisBottom(x))
      .selectAll("text")
          .style("text-anchor", "end")
          .attr("dx", "-.8em")
          .attr("dy", ".15em")
          .attr("transform", "rotate(-65)");

    // Add X axis label
    svg.append("text")
      .attr("transform",
            "translate(" + (width/2) + " ," +
                           (height + margin.top + 40) + ") ")
      .style("text-anchor", "middle")
      .text("Time");


    // Add Y axis
    //minY = d3.min(data, function(d) { return +d.light} );
    //maxY = d3.max(data, function(d) { return +d.light} );

    var y = d3.scaleLinear()
      .domain([0, 1024])
      .range([height, 0]);
    svg.append("g")
      .call(d3.axisLeft(y));

    // Add Y axis label
    svg.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 - margin.left)
      .attr("x", 0 - (height / 2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Light Level");

    // Add a line
    svg.append("path")
      .datum(data)
      .attr("fill", "none")
      .attr("stroke", "brown")
      .attr("stroke-width", 1.5)
      .attr("d", d3.line()
        .x(function(d) { return x(d.date) })
        .y(function(d) { return y(d.light) })
      )
  })
}

function draw_humidity_graph(data) {
  data.then(function(data) {
    // set dimensions and margins
    var margin = {top: 50, right: 30, bottom: 70, left: 60 },
      width = 460 - margin.left - margin.right,
      height = 400 - margin.top - margin.bottom;

    // Create svg
    var svg = d3.select("#humidity_graph")
      .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
      .append("g")
        .attr("transform",
              "translate(" + margin.left + "," + margin.top + ")");

    // Add Graph title
    svg.append("text")
      .attr("x", (width / 2 ))
      .attr("y", 0 - (margin.top / 2))
      .attr("text-anchor", "middle")
      .style("font-size", "1.4em")
      .style("text-decoration", "underline")
      .text("Humidity");

    // Add X axis
    var x = d3.scaleTime()
      .domain(d3.extent(data, function(d) { return d.date; }))
      .range([0, width]);
    svg.append("g")
      .attr("transform", "translate(0," + height + ")")
      .call(d3.axisBottom(x))
      .selectAll("text")
          .style("text-anchor", "end")
          .attr("dx", "-.8em")
          .attr("dy", ".15em")
          .attr("transform", "rotate(-65)");

    // Add X axis label
    svg.append("text")
      .attr("transform",
            "translate(" + (width/2) + " ," +
                           (height + margin.top + 40) + ") ")
      .style("text-anchor", "middle")
      .text("Time");


    // Add Y axis
    //minY = d3.min(data, function(d) { return +d.humidit} );
    //maxY = d3.max(data, function(d) { return +d.temp } );

    var y = d3.scaleLinear()
      .domain([0, 100])
      .range([height, 0]);
    svg.append("g")
      .call(d3.axisLeft(y));

    // Add Y axis label
    svg.append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 0 - margin.left)
      .attr("x", 0 - (height / 2))
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Relative Humidity");

    // Add a line
    svg.append("path")
      .datum(data)
      .attr("fill", "none")
      .attr("stroke", "brown")
      .attr("stroke-width", 1.5)
      .attr("d", d3.line()
        .x(function(d) { return x(d.date) })
        .y(function(d) { return y(d.humidity) })
      )
  })
}
