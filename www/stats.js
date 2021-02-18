// Read data
var data = d3.json("data/sensor_data.json");

var unit = localStorage.getItem("temp_unit");
if (unit == null) {
  console.log("unit was ", unit);
  // select Farenheit by default
  d3.select('#farenheit').attr('checked', 'true');
  d3.select('#celsius').attr('checked', null);
  // store the value in localstorage
  localStorage.setItem("temp_unit", 'true');
} else {
  // There's a value there,
  // check it and check the correct unit
  console.log("Unit wasn't null");
  setRadio(unit);
}

const button = d3.selectAll('input[name="units"]');

// Draw all dynamic elements
console.log("units? ", button.property("checked"));
draw_latest_data(data, button.property("checked"));
draw_temp_graph(data, button.property("checked"));
draw_light_graph(data);
draw_humidity_graph(data);

// When celsius/farenhiet are modifed, update the
// elements that involve temperature
button.on('change', function(d) {
  var isTrue = (this.value === 'true');
  console.log("button changed to ", isTrue);
  localStorage.setItem("temp_unit", isTrue);
  d3.select("#temp_graph").select("svg").remove();
  d3.select("#curr_data").selectAll("span").remove();
  d3.select("#curr_data").selectAll("svg").remove();
  d3.select("#curr_data").selectAll("circle").remove();
  draw_temp_graph(data, isTrue);
  draw_latest_data(data, isTrue);
})

function setRadio(unit) {
    if (unit === 'true') {
      console.log('Unit was true', unit, 'checking farenheit');
      // select Farenheit
      d3.select('#farenheit').attr('checked', 'true');
      d3.select('#celsius').attr('checked', null);
      // store the value in localstorage
      console.log("storing in cache");
      localStorage.setItem("temp_unit", 'true');
    } else {
      console.log("unit was false", unit, "check celsius");
      // select celsius
      d3.select('#celsius').attr('checked', 'true');
      d3.select('#farenheit').attr('checked', null);
      // store value in localstroage
      console.log("storing in cahce");
      localStorage.setItem('temp_unit', 'false');
  }
}

// Draw the current data element
function draw_latest_data(data, isFarenheit) {
  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
      if (isFarenheit) {
        d.t_temp = (d.temp *(9.0/5.0) + 32).toFixed(2);
      } else {
        d.t_temp = d.temp;
      }
    })
    // geting the lastest data only
    data = data[data.length - 1];
    // Selecting desired div and binding data to it
    para = d3.selectAll("#curr_data")
      .datum(data);

    // Append spans for each data item 
    para.append("span")
      .text( function (d) { return "Temp: " + d.t_temp + "\u00B0"; });
    para.append("span")
      .text( function (d) { 
        return "Humidity: " + Math.round(d.humidity) + "%"; 
      });
    para.append("span")
      .text( function (d) { return "Light Level: " + d.light; });

    // For the heater / light status, I'm adding a green/red circle
    // based on whether they are on or not.
    div = para.append("span").attr("id", "heat");
    div.append("span")
      .text( function (d) { return "Heater "; });
    div.append("svg")
        .attr("width", 20)
        .attr("height", 20)
      .append("circle")
        .attr("r", 10)
        .attr("cx", 10)
        .attr("cy", 10);
    if (data.heat) {
      div.select("svg").select("circle")
        .attr("fill", "#879826");
    } else {
      div.select("svg").select("circle")
        .attr("fill", "#E32C33");
    }

    // Doing the same thing as above but for lights
    div = para.append("span").attr("id", "lights");
    div.append("span")
      .text( function (d) { return "Lights "; });
    div.append("svg")
        .attr("width", 20)
        .attr("height", 20)
      .append("circle")
        .attr("r", 10)
        .attr("cx", 10)
        .attr("cy", 10);
    if (data.light_status) {
      div.select("svg").select("circle")
        .attr("fill", "#879826");
    } else {
      div.select("svg").select("circle")
        .attr("fill", "#E32C33");
    }
  })
}

// draw the temperature over time graph
function draw_temp_graph(data, isFarenheit) {
  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
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
    .domain(d3.extent(data, function(d) { return d.d_date; }))
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
    .attr("stroke", "#00a899")
    .attr("stroke-width", 1.5)
    .attr("d", d3.line()
      .x(function(d) { return x(d.d_date) })
      .y(function(d) { return y(d.t_temp) })
    )
  })
}

// Draw the light over time graph
function draw_light_graph(data) {
  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
    })
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
      .domain(d3.extent(data, function(d) { return d.d_date; }))
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
      .attr("stroke", "#e10082")
      .attr("stroke-width", 1.5)
      .attr("d", d3.line()
        .x(function(d) { return x(d.d_date) })
        .y(function(d) { return y(d.light) })
      )
  })
}

// Draw the humidity over time graph
function draw_humidity_graph(data) {
  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
    })
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
      .domain(d3.extent(data, function(d) { return d.d_date; }))
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
      .attr("stroke", "#b59000")
      .attr("stroke-width", 1.5)
      .attr("d", d3.line()
        .x(function(d) { return x(d.d_date) })
        .y(function(d) { return y(d.humidity) })
      )
  })
}