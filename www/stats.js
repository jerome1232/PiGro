// Read data
const button = d3.selectAll('input[name="units"]');

// Register an interval to reload the graphs every so often
var intervalId = window.setInterval(function() {
  update();
}, 60000)

// When celsius/fahrenheit are modified, update the
// elements that involve temperature
button.on('change', function(d) {
  const isTrue = (this.value === 'true');
  localStorage.setItem("temp_unit", isTrue);
  d3.select("#temp_graph").selectAll("svg").remove();
  d3.select("#curr_data").html(null);
  draw_temp_graph();
  draw_latest_data();
})

var data = d3.json("data/sensor_data.json");

// load fahrenheit/celsius units from cache
var unit = localStorage.getItem("temp_unit");

// if it wasn't in cache, go fahrenheit by default
if (unit === null) {
  console.log("Unit was false, set to true", unit);
  setRadio(unit);
} else {
  // There's a value there,
  // check it and check the correct unit
  unit = unit === 'true'; // converting string to boolean
  setRadio(unit);
}

// Draw all dynamic elements
draw_latest_data(data);
draw_temp_graph(data);
draw_light_graph(data);
draw_humidity_graph(data);
draw_moisture_1(data);
draw_moisture_2(data);


function update() {
  var data = d3.json("data/sensor_data.json");

  // Removing all traces of the old
  d3.select("#temp_graph").selectAll("svg").remove();
  d3.select("#curr_data").html(null);
  d3.select("#light_graph").selectAll("svg").remove();
  d3.select("#humidity_graph").selectAll("svg").remove();
  d3.select("#moisture_1_graph").selectAll("svg").remove();
  d3.select("#moisture_2_graph").selectAll("svg").remove();

  // drawing the new
  draw_latest_data(data);
  draw_temp_graph(data);
  draw_light_graph(data);
  draw_humidity_graph(data);
  draw_moisture_1(data);
  draw_moisture_2(data);
}

function setRadio(unit) {
    if (unit) {
      // select Fahrenheit
      d3.select('#fahrenheit').attr('checked', 'true');
      d3.select('#celsius').attr('checked', null);
      // store the value in localstorage
      localStorage.setItem("temp_unit", true);
    } else {
      // select celsius
      d3.select('#celsius').attr('checked', 'true');
      d3.select('#fahrenheit').attr('checked', null);
      // store value in localstroage
      localStorage.setItem('temp_unit', false);
  }
}

var formatTime = d3.timeFormat("%I:%M %p");

// Draw the current data element
function draw_latest_data(data) {

  const button = d3.selectAll('input[name="units"]');
  // var data = d3.json("data/sensor_data.json");
  const isFahrenheit = button.property('checked');

  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
      if (isFahrenheit) {
        d.t_temp = (d.temp *(9.0/5.0) + 32).toFixed(2);
      } else {
        d.t_temp = d.temp;
      }
    })
    // getting the latest data only
    data = data[data.length - 1];
    // Selecting desired div and binding data to it
    para = d3.selectAll("#curr_data")
      .datum(data);

    var t_format = d3.timeFormat('%a %b %e %I:%M %p')

    // Append spans for each data item
    para.append("span")
      .text(function (d) { return t_format(d.d_date); });
    para.append("br");
    para.append('hr');
    para.append("span")
      .text( function (d) { return "Temp: " + d.t_temp + "\u00B0"; });
    para.append("span")
      .text( function (d) {
        return "Humidity: " + Math.round(d.humidity) + "%";
      });
    para.append("span")
      .text( function (d) { return "Light Level: " + d.light; });
    para.append("br");
    para.append("span")
      .text( function (d) { return "Moisture Bay 1: " + Math.abs(d.soil_moisture_1 / 1023 * 100 - 100).toFixed(2) + "%"; });
    para.append("span")
      .text( function (d) { return "Moisture Bay 2: " + Math.abs(d.soil_moisture_2 / 1023 * 100 - 100).toFixed(2) + "%"; });
    para.append('br');

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
function draw_temp_graph(data) {

  const button = d3.selectAll('input[name="units"]');
  // var data = d3.json("data/sensor_data.json");
  const isFahrenheit = button.property('checked');

  // Creating tooltip div
  var div = d3.select("#temp_graph").append("div")
  .attr("class", "tooltip")
  .style("opacity", 0);

  // Handling data when it's arrived
  data.then(function (data) {
    data.forEach( (d) => {
      d.d_date = new Date(d.time_stamp * 1000);
      if (isFahrenheit) {
        d.t_temp = (d.temp * (9.0 / 5.0) + 32).toFixed(2);
      } else {
        d.t_temp = d.temp;
      }
    });

    // set dimensions and margins
    var margin = {top: 50, right: 30, bottom: 100, left: 60 },
      width = 800 - margin.left - margin.right,
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
      .attr('id', 'temp_line')
      .attr("d", d3.line()
        .x(function(d) { return x(d.d_date) })
        .y(function(d) { return y(d.t_temp) })
      )

    // Add dots with tooltips
    svg.selectAll('dot')
      .data(data)
    .enter().append('circle')
      .attr('r', 1.5)
      .attr('cx', function(d) { return x(d.d_date); })
      .attr('cy', function(d) { return y(d.t_temp); })
      .attr('fill', '#00A198')
      .on('mouseover', function(event, d) {
        div.transition()
          .duration(200)
          .style('opacity', .9);
        div.html(formatTime(d.d_date) + '<br/>' + d.t_temp + '\u00B0')
          .style('left', (event.pageX) + 'px')
          .style('top', (event.pageY - 28) + 'px');
      })
      .on('mouseout', function(d) {
        div.transition()
          .duration(500)
          .style('opacity', 0);
      })
  })
}

// Draw the light over time graph
function draw_light_graph(data) {

  // var data = d3.json("data/sensor_data.json");

  var div = d3.select("#light_graph").append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
    })
    // set dimensions and margins
    var margin = {top: 50, right: 30, bottom: 70, left: 60 },
      width = 800 - margin.left - margin.right,
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

    // Add dots with tooltips
    svg.selectAll('dot')
      .data(data)
    .enter().append('circle')
      .attr('r', 1.5)
      .attr('cx', function(d) { return x(d.d_date); })
      .attr('cy', function(d) { return y(d.light); })
      .attr('fill', '#D93480')
      .on('mouseover', function(event, d) {
        div.transition()
          .duration(200)
          .style('opacity', .9);
        div.html(formatTime(d.d_date) + '<br/>' + d.light)
          .style('left', (event.pageX) + 'px')
          .style('top', (event.pageY - 28) + 'px');
      })
      .on('mouseout', function(d) {
        div.transition()
          .duration(500)
          .style('opacity', 0);
      })
  })
}

// Draw the humidity over time graph
function draw_humidity_graph(data) {

  // var data = d3.json("data/sensor_data.json");

  var div = d3.select("#humidity_graph").append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
    })
    // set dimensions and margins
    var margin = {top: 50, right: 30, bottom: 70, left: 60 },
      width = 800 - margin.left - margin.right,
      height = 460 - margin.top - margin.bottom;

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

    // Add dots with tooltips
    svg.selectAll('dot')
      .data(data)
    .enter().append('circle')
      .attr('r', 1.5)
      .attr('cx', function(d) { return x(d.d_date); })
      .attr('cy', function(d) { return y(d.humidity); })
      .attr('fill', '#b59000')
      .on('mouseover', function(event, d) {
        div.transition()
          .duration(200)
          .style('opacity', .9);
        div.html(formatTime(d.d_date) + '<br/>' + Math.round(d.humidity) + '%')
          .style('left', (event.pageX) + 'px')
          .style('top', (event.pageY - 28) + 'px');
      })
      .on('mouseout', function(d) {
        div.transition()
          .duration(500)
          .style('opacity', 0);
      })

  })
}

// Draw the humidity over time graph
function draw_moisture_1(data) {

  // var data = d3.json("data/sensor_data.json");

  var div = d3.select("#humidity_graph").append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
      d.d_soil_moisture_1 = 100 - (d.soil_moisture_1 / 1023 * 100)
    })
    // set dimensions and margins
    var margin = {top: 50, right: 30, bottom: 70, left: 60 },
      width = 800 - margin.left - margin.right,
      height = 460 - margin.top - margin.bottom;

    // Create svg
    var svg = d3.select("#moisture_1_graph")
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
      .text("Soil Moisture: Bay 1");

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
      .text("Soil Moisture");

    // Add a line
    svg.append("path")
      .datum(data)
      .attr("fill", "none")
      .attr("stroke", "#6573C0")
      .attr("stroke-width", 1.5)
      .attr("d", d3.line()
        .x(function(d) { return x(d.d_date) })
        .y(function(d) { return y(d.d_soil_moisture_1) })
      )

    // Add dots with tooltips
    svg.selectAll('dot')
      .data(data)
    .enter().append('circle')
      .attr('r', 1.5)
      .attr('cx', function(d) { return x(d.d_date); })
      .attr('cy', function(d) { return y(d.d_soil_moisture_1); })
      .attr('fill', '#6573C0')
      .on('mouseover', function(event, d) {
        div.transition()
          .duration(200)
          .style('opacity', .9);
        div.html(formatTime(d.d_date) + '<br/>' + Math.round(d.d_soil_moisture_1) + '%')
          .style('left', (event.pageX) + 'px')
          .style('top', (event.pageY - 28) + 'px');
      })
      .on('mouseout', function(d) {
        div.transition()
          .duration(500)
          .style('opacity', 0);
      })
  })
}
// Draw the humidity over time graph
function draw_moisture_2(data) {

  // var data = d3.json("data/sensor_data.json");

  var div = d3.select("#humidity_graph").append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

  data.then(function(data) {
    data.forEach(function(d) {
      d.d_date = new Date(d.time_stamp * 1000);
      d.d_soil_moisture_2 = 100 - (d.soil_moisture_2 / 1023 * 100)
    })
    // set dimensions and margins
    var margin = {top: 50, right: 30, bottom: 70, left: 60 },
      width = 800 - margin.left - margin.right,
      height = 460 - margin.top - margin.bottom;

    // Create svg
    var svg = d3.select("#moisture_2_graph")
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
      .text("Soil Moisture: Bay 2");

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
      .text("Soil Moisture");

    // Add a line
    svg.append("path")
      .datum(data)
      .attr("fill", "none")
      .attr("stroke", "#D14821")
      .attr("stroke-width", 1.5)
      .attr("d", d3.line()
        .x(function(d) { return x(d.d_date) })
        .y(function(d) { return y(Math.abs(d.soil_moisture_2 / 1023 * 100 - 100)) })
      )

    // Add dots with tooltips
    svg.selectAll('dot')
      .data(data)
    .enter().append('circle')
      .attr('r', 1.5)
      .attr('cx', function(d) { return x(d.d_date); })
      .attr('cy', function(d) { return y(d.d_soil_moisture_2); })
      .attr('fill', '#D14821')
      .on('mouseover', function(event, d) {
        div.transition()
          .duration(200)
          .style('opacity', .9);
        div.html(formatTime(d.d_date) + '<br/>' + Math.round(d.d_soil_moisture_2) + '%')
          .style('left', (event.pageX) + 'px')
          .style('top', (event.pageY - 28) + 'px');
      })
      .on('mouseout', function(d) {
        div.transition()
          .duration(500)
          .style('opacity', 0);
      })
  })
}