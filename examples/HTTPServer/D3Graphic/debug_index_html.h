#include <Arduino.h>
#include "http_common.h"

const uint8_t webserver_DIR_debug_DIR_index_html_data[9141] = "<!DOCTYPE html>\n" \
"<meta charset=\"utf-8\">\n" \
"<style>\n" \
"\n" \
"    body {\n" \
"        font: 10px sans-serif;\n" \
"    }\n" \
"\n" \
"    .axis path,\n" \
"    .axis line {\n" \
"        fill: none;\n" \
"        stroke: #000;\n" \
"        shape-rendering: crispEdges;\n" \
"    }\n" \
"\n" \
"    .bar {\n" \
"        fill: steelblue;\n" \
"    }\n" \
"\n" \
"    .x.axis path {\n" \
"        display: none;\n" \
"    }\n" \
"\n" \
"    .container {\n" \
"        width: 1080px;\n" \
"        margin: auto;\n" \
"    }\n" \
"\n" \
"    .thread_chart_container {\n" \
"        width: 700px;\n" \
"        float: left;\n" \
"    }\n" \
"\n" \
"    .heap_chart_container {\n" \
"        width: 300px;\n" \
"        float: left;\n" \
"        margin: 160px 0 0 80px;\n" \
"    }\n" \
"\n" \
"    #heap_chart {\n" \
"        margin-bottom: 50px;\n" \
"    }\n" \
"\n" \
"    #heap_chart {\n" \
"\n" \
"    }\n" \
"\n" \
"    h1, h2 {\n" \
"        text-align: center;\n" \
"    }\n" \
"\n" \
"</style>\n" \
"<body>\n" \
"<h1>Debug Information</h1>\n" \
"<div class=\"container\">\n" \
"    <div class=\"thread_chart_container\">\n" \
"        <div id=\"thread_chart\"></div>\n" \
"        <h2>Thread Memory</h2>\n" \
"    </div>\n" \
"    <div class=\"heap_chart_container\">\n" \
"        <div id=\"heap_chart\"></div>\n" \
"        <h2>Heap Memory</h2>\n" \
"    </div>\n" \
"</div>\n" \
"\n" \
"\n" \
"<script src=\"/d3.min.js\"></script>\n" \
"<script>\n" \
"    function ajaxCall(path, success) {\n" \
"        var xhr = new XMLHttpRequest();\n" \
"        xhr.onreadystatechange = function()\n" \
"        {\n" \
"            if (xhr.readyState === XMLHttpRequest.DONE) {\n" \
"                if (xhr.status === 200) {\n" \
"                    if (success)\n" \
"                        success(xhr.responseText);\n" \
"                } else {\n" \
"                    alert(\'Can not load data\')\n" \
"                }\n" \
"            }\n" \
"        };\n" \
"        xhr.open(\"GET\", path, true);\n" \
"        xhr.send();\n" \
"    }\n" \
"    function drawThreadChart() {\n" \
"        var margin = {top: 20, right: 20, bottom: 100, left: 40},\n" \
"                width = 700 - margin.left - margin.right,\n" \
"                height = 500 - margin.top - margin.bottom;\n" \
"\n" \
"        var x = d3.scale.ordinal()\n" \
"                .rangeRoundBands([0, width], .1);\n" \
"\n" \
"        var y = d3.scale.linear()\n" \
"                .rangeRound([height, 0]);\n" \
"\n" \
"        var color = d3.scale.ordinal()\n" \
"                .range([\"#009966\", \"#ddd\"]);\n" \
"\n" \
"        var xAxis = d3.svg.axis()\n" \
"                .scale(x)\n" \
"                .orient(\"bottom\");\n" \
"\n" \
"        var yAxis = d3.svg.axis()\n" \
"                .scale(y)\n" \
"                .orient(\"left\")\n" \
"                .tickFormat(d3.format(\".2s\"));\n" \
"\n" \
"        var svg = d3.select(\"#thread_chart\").append(\"svg\")\n" \
"                .attr(\"width\", width + margin.left + margin.right)\n" \
"                .attr(\"height\", height + margin.top + margin.bottom)\n" \
"                .append(\"g\")\n" \
"                .attr(\"transform\", \"translate(\" + margin.left + \",\" + margin.top + \")\");\n" \
"\n" \
"        d3.csv(\"/debug/thread.csv\", function(error, data) {\n" \
"            if (error) throw error;\n" \
"\n" \
"            color.domain(d3.keys(data[0]).filter(function(key) { return key !== \"Name\"; }));\n" \
"\n" \
"            data.forEach(function(d) {\n" \
"                var y0 = 0;\n" \
"                d.bytes = color.domain().map(function(name) { return {name: name, y0: y0, y1: y0 += +d[name]}; });\n" \
"                d.total = d.bytes[d.bytes.length - 1].y1;\n" \
"            });\n" \
"\n" \
"            data.sort(function(a, b) { return b.total - a.total; });\n" \
"\n" \
"            x.domain(data.map(function(d) { return d.Name; }));\n" \
"            y.domain([0, d3.max(data, function(d) { return d.total; })]);\n" \
"\n" \
"            svg.append(\"g\")\n" \
"                    .attr(\"class\", \"x axis\")\n" \
"                    .attr(\"transform\", \"translate(0,\" + height + \")\")\n" \
"                    .call(xAxis)\n" \
"//                    .selectAll(\'.x .tick text\') // select all the x tick texts\n" \
"//                    .call(function(t){\n" \
"//                        t.each(function(d){ // for each one\n" \
"//                            var self = d3.select(this);\n" \
"//                            var s = self.text().split(\"|\");  // get the text and split it\n" \
"//                            self.text(\'\'); // clear it out\n" \
"//                            self.append(\"tspan\") // insert two tspans\n" \
"//                                    .attr(\"x\", 0)\n" \
"//                                    .attr(\"dy\",\".8em\")\n" \
"//                                    .text(s[0]);\n" \
"//                            self.append(\"tspan\")\n" \
"//                                    .attr(\"x\", 0)\n" \
"//                                    .attr(\"dy\",\".8em\")\n" \
"//                                    .text(s[1]);\n" \
"//                        })\n" \
"//                    });\n" \
"                    .selectAll(\"text\")\n" \
"                    .style(\"text-anchor\", \"start\")\n" \
"                    .attr(\"dx\", \".8em\")\n" \
"                    .attr(\"dy\", \".15em\")\n" \
"                    .attr(\"transform\", \"rotate(65)\");\n" \
"\n" \
"\n" \
"            svg.append(\"g\")\n" \
"                    .attr(\"class\", \"y axis\")\n" \
"                    .call(yAxis)\n" \
"                    .append(\"text\")\n" \
"                    .attr(\"transform\", \"rotate(-90)\")\n" \
"                    .attr(\"y\", 0)\n" \
"                    .attr(\"dy\", \".71em\")\n" \
"                    .style(\"text-anchor\", \"end\")\n" \
"                    .text(\"Bytes\");\n" \
"\n" \
"            var state = svg.selectAll(\".state\")\n" \
"                    .data(data)\n" \
"                    .enter().append(\"g\")\n" \
"                    .attr(\"class\", \"g\")\n" \
"                    .attr(\"transform\", function(d) { return \"translate(\" + x(d.Name) + \",0)\"; });\n" \
"\n" \
"            state.selectAll(\"rect\")\n" \
"                    .data(function(d) { return d.bytes; })\n" \
"                    .enter().append(\"rect\")\n" \
"                    .attr(\"width\", x.rangeBand())\n" \
"                    .attr(\"y\", function(d) { return y(d.y1); })\n" \
"                    .attr(\"height\", function(d) { return y(d.y0) - y(d.y1); })\n" \
"                    .style(\"fill\", function(d) { return color(d.name); });\n" \
"\n" \
"            var legend = svg.selectAll(\".legend\")\n" \
"                    .data(color.domain().slice().reverse())\n" \
"                    .enter().append(\"g\")\n" \
"                    .attr(\"class\", \"legend\")\n" \
"                    .attr(\"transform\", function(d, i) { return \"translate(0,\" + i * 20 + \")\"; });\n" \
"\n" \
"            legend.append(\"rect\")\n" \
"                    .attr(\"x\", width - 18)\n" \
"                    .attr(\"width\", 18)\n" \
"                    .attr(\"height\", 18)\n" \
"                    .style(\"fill\", color);\n" \
"\n" \
"            legend.append(\"text\")\n" \
"                    .attr(\"x\", width - 24)\n" \
"                    .attr(\"y\", 9)\n" \
"                    .attr(\"dy\", \".35em\")\n" \
"                    .style(\"text-anchor\", \"end\")\n" \
"                    .text(function(d) { return d; });\n" \
"\n" \
"        });\n" \
"    }\n" \
"    function updateDrawThreadChart() {\n" \
"        d3.select(\"#thread_chart\").selectAll(\"svg\").remove()\n" \
"        drawThreadChart()\n" \
"    }\n" \
"    function drawHeapChart() {\n" \
"        d3.csv(\"/debug/heap.csv\", function(error, data) {\n" \
"            if (error) throw error;\n" \
"            var width = 300;\n" \
"            var height = 300;\n" \
"            var radius = Math.min(width, height) / 2;\n" \
"            var donutWidth = 80;\n" \
"            var legendRectSize = 18;\n" \
"            var legendSpacing = 4;\n" \
"\n" \
"            var color = d3.scale.category20b();\n" \
"\n" \
"            var svg = d3.select(\'#heap_chart\')\n" \
"                    .append(\'svg\')\n" \
"                    .attr(\'width\', width)\n" \
"                    .attr(\'height\', height)\n" \
"                    .append(\'g\')\n" \
"                    .attr(\'transform\', \'translate(\' + (width / 2) +\n" \
"                    \',\' + (height / 2) + \')\');\n" \
"\n" \
"            var arc = d3.svg.arc()\n" \
"                    .innerRadius(radius - donutWidth)\n" \
"                    .outerRadius(radius);\n" \
"\n" \
"            var pie = d3.layout.pie()\n" \
"                    .value(function (d) {\n" \
"                        return d.count;\n" \
"                    })\n" \
"                    .sort(null);\n" \
"\n" \
"            var path = svg.selectAll(\'path\')\n" \
"                    .data(pie(data))\n" \
"                    .enter()\n" \
"                    .append(\'path\')\n" \
"                    .attr(\'d\', arc)\n" \
"                    .attr(\'fill\', function (d, i) {\n" \
"                        return color(d.data.label);\n" \
"                    });\n" \
"\n" \
"            var legend = svg.selectAll(\'.legend\')\n" \
"                    .data(color.domain())\n" \
"                    .enter()\n" \
"                    .append(\'g\')\n" \
"                    .attr(\'class\', \'legend\')\n" \
"                    .attr(\'transform\', function (d, i) {\n" \
"                        var height = legendRectSize + legendSpacing;\n" \
"                        var offset = height * color.domain().length / 2;\n" \
"                        var horz = -2 * legendRectSize;\n" \
"                        var vert = i * height - offset;\n" \
"                        return \'translate(\' + horz + \',\' + vert + \')\';\n" \
"                    });\n" \
"\n" \
"            legend.append(\'rect\')\n" \
"                    .attr(\'width\', legendRectSize)\n" \
"                    .attr(\'height\', legendRectSize)\n" \
"                    .style(\'fill\', color)\n" \
"                    .style(\'stroke\', color);\n" \
"\n" \
"            legend.append(\'text\')\n" \
"                    .attr(\'x\', legendRectSize + legendSpacing)\n" \
"                    .attr(\'y\', legendRectSize - legendSpacing)\n" \
"                    .text(function (d) {\n" \
"                        return d;\n" \
"                    });\n" \
"        });\n" \
"    }\n" \
"    function updateHeapChart() {\n" \
"        d3.select(\"#heap_chart\").selectAll(\"svg\").remove();\n" \
"        drawHeapChart()\n" \
"    }\n" \
"\n" \
"    drawThreadChart()\n" \
"    drawHeapChart()\n" \
"    setInterval(updateHeapChart, 5000);\n" \
"    setInterval(updateDrawThreadChart, 5000);\n" \
"</script>";

//const resource_hnd_t webserver_DIR_debug_DIR_index_html = { RESOURCE_IN_MEMORY, 9140, { .mem = { webserver_DIR_debug_DIR_index_html_data }}};
const HTTPResource webserver_DIR_debug_DIR_index_html(webserver_DIR_debug_DIR_index_html_data,  9140);
