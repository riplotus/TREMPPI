/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global w2ui */

tremppi = {
    widgetInterface: function () {
        return {
            page: function () {
                tremppi.log("page not implemented", "warning");
            },
            layout: function () {
                tremppi.log("layout not implemented", "warning");
            },
            setData: function (data) {
                tremppi.log("setData not implemented", "warning");
            }
        };
    },
    getServerAddress: function () {
        return "http://" + tremppi.setup.server_location + ":" + tremppi.setup.server_port + "/";
    },
    makeDataFilepath: function (filename) {
        if (typeof filename === 'undefined') {
            return "./data/" + tremppi.widget_name + ".json";
        } else {
            return "./data/" + tremppi.widget_name + "/" + filename + ".json";
        }
    },
    saveData: function (data, filename) {
        if (typeof data === 'undefined') {
            tremppi.log("no data specified in saveData", "error");
        }
        var content = JSON.stringify(data, null, '\t');
        var url = tremppi.getServerAddress() + "?" + tremppi.makeDataFilepath(filename);
        $.ajax({
            type: "POST",
            url: url,
            data: content,
            success: function (res) {
                tremppi.log(filename + " saved successfully.");
            },
            fail: function (res) {
                tremppi.log("Load of the file " + filename + " failed! Data were not saved!");
            }
        });
    },
    getData: function (callback, filename) {
        $.getJSON(tremppi.makeDataFilepath(filename), callback)
                .done(function () {
                    tremppi.log(filename + " loaded successfully.");
                })
                .fail(function (jqXHR, textStatus, errorThrown) {
                    tremppi.log("Load of the file " + filename + " failed!");
                });
    },
    makeStorageKey: function (key) {
        return "tremppi." + tremppi.project_name + "." + tremppi.widget_name + "." + key;
    },
    setItem: function (key, value) {
        if (typeof localStorage === 'undefined') {
            tremppi.log("localStorage not available, browser key " + key + " not stored", "warning");
        }
        else {
            localStorage.setItem(makeStorageKey(key), value);
        }
    },
    getItem: function (key, value) {
        if (typeof localStorage === 'undefined') {
            tremppi.log("localStorage not available, browser key " + key + " not loaded", "warning");
            return null;
        }
        else {
            return localStorage.getItem(makeStorageKey(key));
        }
    },
    makeScript: function(src) {
        var element = document.createElement('script');
        element.src = src;
        return element;
    },
    makeLink: function(rel, type, href) {
        var element = document.createElement('link');
        element.rel = rel;
        element.type = type;
        element.href = href;
        return element;
    },
    makeHead: function () {
        var head = document.getElementsByTagName('head')[0];
        
        // libraries 
        head.appendChild(tremppi.makeScript('./libs/jquery-2.1.3.js'));
        head.appendChild(tremppi.makeScript('./libs/cytoscape-2.3.9.js'));
        head.appendChild(tremppi.makeScript('./libs/w2ui-1.4.2.js'));
        head.appendChild(tremppi.makeLink('stylesheet', 'text/css', './libs/w2ui-1.4.2.css'));
        
        // common 
        head.appendChild(tremppi.makeLink('icon', 'image/x-icon', './favicon.ico'));
        head.appendChild(tremppi.makeScript('./setup.js?_=' + Math.random().toString().slice(2))); // load the setup with cache busing
        head.appendChild(tremppi.makeLink('stylesheet', 'text/css', './common/css.css'));
        head.appendChild(tremppi.makeScript('./common/common.js'));
        
        // widget-related
        head.appendChild(tremppi.makeLink('stylesheet', 'text/css', './' + tremppi.widget_name + '/css.css'));
        head.appendChild(tremppi.makeScript('./' + tremppi.widget_name + '/widget.js'));
        head.appendChild(tremppi.makeScript('./' + tremppi.widget_name + '/controls.js'));
        head.appendChild(tremppi.makeScript('./' + tremppi.widget_name + '/page.js'));
        head.appendChild(tremppi.makeScript('./data/' + tremppi.widget_name + '.js?_=' + Math.random().toString().slice(2)));
        
        document.title = tremppi.widget_name;
    },
    makeBody: function () {
        var sidebar = {
            name: 'sidebar',
            nodes: [
                {id: 'widget_list', text: 'widgets', expanded: true, group: true,
                    nodes: [
                        {id: 'index', text: 'index'},
//                        {id: 'editor', text: 'editor'},
//                        {id: 'select', text: 'select'},
//                        {id: 'properties', text: 'properties'},
//                        {id: 'quantitative', text: 'quantitative'},
//                        {id: 'qualitative', text: 'qualitative'}
                    ]
                }
            ]
        };

        // Set basic layout
        var layout_style = 'border: 0px solid #dfdfdf;';
        var layout = {
            name: 'layout',
            panels: [
                {type: 'left', style: layout_style, size: 200, content: '<div id="files" ></div>'},
                {type: 'main', style: layout_style, content: '<div id="widget" ></div>'},
                {type: 'bottom', size: 20, content: '<div id="log_line" ></div>'}
            ]
        };
        $('body').w2layout(layout);

        // Set left side bar
        w2ui.layout.content('left', $().w2sidebar(sidebar));
        var sidebar = w2ui.layout.get('left').content.sidebar;
        sidebar.select(tremppi.widget_name);
        sidebar.on('click', function (event) {
            window.open("./" + event.target + ".html", "_self");
        });

        // Set the toolbar, if any
        if (toolbar !== "undefined")
            layout.panels[1].toolbar = tremppi.widget.toolbar;
        tremppi.toolbar = w2ui.layout.get("main").toolbar;

        // Set the widget
        tremppi.widget.page();
        w2ui.layout.on('resize', tremppi.widget.layout);

        // Load the specific data
        $.ajaxSetup({cache: false});
        tremppi.getData(tremppi.widget.setData);
    }
};
// Initial content execution, 
var url = window.location.pathname;
tremppi.widget_name = url.substring(url.lastIndexOf('/') + 1).slice(0, -5);
tremppi[tremppi.widget_name] = tremppi.widget = tremppi.widgetInterface();
tremppi.makeHead();
window.onload = tremppi.makeBody;


