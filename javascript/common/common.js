/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* global tremppi */

tremppi.common = {
    compare_numbers: function (a, b) {
        return a - b;
    }
};
tremppi.qtip = {
    getConfig: function () {
        this.position = {
            target: 'mouse', // Position it where the click was...
            adjust: {mouse: false} // ...but don't follow the mouse
        };
        this.content = {text: ''};
        this.show = false; // Do not show on mouseover of the graph
    },
    addOnHoverLabeller: function (div_id, elements, labeller) {
        var config = new tremppi.qtip.getConfig();
        var api = $('#' + div_id).qtip(config).qtip('api');
        elements.on('tapdragover', function (eve) {
            var my_data = eve.cyTarget.data();
            api.set('content.text', labeller(my_data));
            api.show();
        });
        elements.on('tapdragout', function () {
            api.hide();
        });
    }
};
tremppi.editablegrid = {
    // test is a row is not empty
    isEmpty: function (columns) {
        for (var i = 0; i < columns.length; i++) {
            if (columns[i] !== '' && columns[i] !== false)
                return false;
        }
        return true;
    }
};
tremppi.w2ui = {
    findByRecID: function (records, recid) {
        return records[tremppi.w2ui.iByRecID(records, recid)];
    },
    iByRecID: function (records, recid) {
        for (var i = 0; i < records.length; i++) {
            if (records[i].recid === recid)
                return i;
        }
        console.log('Error. Have not found element ' + recid);
    },
    getFreeRecID: function (records) {
        var ids = records.map(function (entry) {
            return parseInt(entry.recid);
        });
        ids.sort(tremppi.common.compare_numbers);
        for (var id = 0; id < ids.length; id++) {
            if (ids[id] !== id) {
                return id;
            }
        }
        return id;
    },
    checkAll: function (event, grid) {
        var new_val = !event.item.checked;
        grid.records.forEach(function (record) {
            record[event.target] = new_val;
        });
    },
    add: function (grid, element) {
        if (typeof element === 'undefined') {
            throw 'trying to add an empty element';
        }
        element.recid = tremppi.w2ui.getFreeRecID(grid.records);
        grid.add(element);
        grid.selectNone();
        grid.select(element.recid);
    },
    deleteSelected: function (grid) {
        grid.getSelection().forEach(function (recid) {
            for (var i = 0; i < grid.records.length; i++) {
                if (grid.records[i].recid === recid) {
                    grid.records.splice(i, 1);
                    break;
                }
            }
        });
        grid.selectNone();
    },
    duplicateSelected: function (grid) {
        var selected = grid.getSelection();
        grid.selectNone();
        selected.forEach(function (recid) {
            for (var i = 0; i < grid.records.length; i++) {
                if (grid.records[i].recid === recid) {
                    var new_entry = {};
                    $.extend(true, new_entry, grid.records[i]);
                    new_entry.recid = tremppi.w2ui.getFreeRecID(grid.records);
                    if (typeof new_entry.name !== 'undefined') {
                        new_entry.name += ' (copy)';
                    }
                    grid.records.splice(i + 1, 0, new_entry);
                    grid.select(new_entry.recid);
                    break;
                }
            }
        });
    },
    up: function (grid) {
        var selection = grid.getSelection();
        var positions = selection.map(function (recid) {
            return tremppi.w2ui.iByRecID(grid.records, recid);
        });
        positions.sort(tremppi.common.compare_numbers);
        grid.selectNone();
        for (var i = 0; i < positions.length; i++) {
            var pos = positions[i];
            if (pos > 0) {
                var temp = grid.records[pos];
                grid.records[pos] = grid.records[pos - 1];
                grid.records[pos - 1] = temp;
            }
        }
        selection.forEach(function (recid) {
            grid.select(recid);
        });
    },
    down: function (grid) {
        var selection = grid.getSelection();
        var positions = selection.map(function (recid) {
            return tremppi.w2ui.iByRecID(grid.records, recid);
        });
        positions.sort(tremppi.common.compare_numbers);
        grid.selectNone();
        for (var i = positions.length - 1; i >= 0; i--) {
            var pos = positions[i];
            console.log(pos);
            if (pos + 1 < grid.records.length) {
                var temp = grid.records[pos];
                grid.records[pos] = grid.records[pos + 1];
                grid.records[pos + 1] = temp;
            }
        }
        selection.forEach(function (recid) {
            grid.select(recid);
        });
    }
};

tremppi.cytoscape = {
    // test if nodes all have positions
    hasAllPositions: function (nodes) {
        if (typeof nodes === 'undefined')
            return true;
        for (var i = 0; i < nodes.length; i++) {
            if (!nodes[i].position)
                return false;
            if (!nodes[i].position.x || !nodes[i].position.y)
                return false;
        }
        return true;
    }
};
tremppi.report = {
    selections: ['left', 'mid', 'right', 'all'],
    addSetup: function (setup) {
        values = ['date', 'name', 'pool_size', 'select', 'selected', 'compare', 'compared'];
        for (var i = 0; i < values.length; i++) {
            $('#analysis_setup').append('<div class="decription"><span class="desc_title">' +
                    values[i] +
                    ':</span> <span class="desc_content" id="analysis_date">' +
                    setup[values[i]] +
                    '</span></div>');
        }
    }, // Synchronization in between the graphs
    synchronize: function (config, labelFunction) {
        var cys = [];
        for (var id = 0; id < config.types.length; id++) {
            cys[id] = $('#graph_' + config.types[id]).cytoscape('get');
        }
        var nodes = cys[0].elements('node');

        // Sets all nodes with the id to the position given by graph
        var moveFunction = function (graph, id) {
            return function (evt) {
                for (var i = 0; i < config.types.length; i++) {
                    cys[i].$(id).renderedPosition(graph.$(id).renderedPosition());
                    tremppi.data[config.types[i]]['elements'] = cys[i].json().elements;
                }
                tremppi.common.save();
            };
        };

        // Set node drag reactions to all
        for (var j = 0; j < nodes.length; j++) {
            var id = '#' + nodes[j].id();
            for (var i = 0; i < config.types.length; i++) {
                cys[i].$(id).on('drag', moveFunction(cys[i], id));
            }
        }

        // Create zooming function
        var zoomFunction = function (graph, id) {
            return function (evt) {
                for (i = 0; i < config.types.length; i++) {
                    if ((id === i)
                            || (cys[i].zoom() === graph.zoom())
                            || (cys[i].pan() === graph.pan()))
                        continue;
                    cys[i].pan(graph.pan());
                    cys[i].zoom(graph.zoom());
                }
                labelFunction();
            };
        };

        var panFunction = function (graph, id) {
            return function (evt) {
                for (i = 0; i < config.types.length; i++) {
                    if ((id === i) || (cys[i].pan() === graph.pan()))
                        continue;
                    cys[i].pan(graph.pan());
                }
                ;
            };
        };
        for (var i = 0; i < config.types.length; i++) {
            cys[i].on('zoom', zoomFunction(cys[i], i));
        }

        for (var i = 0; i < config.types.length; i++) {
            cys[i].on('mouseup', panFunction(cys[i], i));
        }
    },
    pickData: function (source, panel) {
        tremppi.getData(tremppi.widget.valuesSetter(source, panel), source);
    },
    createPanels: function () {
        tremppi.toolbar.get('select').items = tremppi.widget.setup.files;
        tremppi.toolbar.get('compare').items = tremppi.widget.setup.files;
        $("#widget").append('<div id="container_left">left</div>');
        $("#widget").append('<div id="container_mid">mid</div>');
        $("#widget").append('<div id="container_right">right</div>');
        tremppi.report.setPanel('left');
        tremppi.report.setPanel('mid');
        tremppi.report.setPanel('right');
    },
    initialPanel: function () {
        var panel = tremppi.getItem('panel', 'all');
        if (panel === 'left' || panel === 'mid' || panel === 'right') {
            tremppi.toolbar.uncheck('all');
            tremppi.toolbar.check(panel);
            tremppi.report.showPanel(panel);
        }

        if (tremppi.getItem('selected') !== null) {
            tremppi.report.pickData(tremppi.getItem('selected'), 'left');
        }
        if (tremppi.getItem('compared') !== null) {
            tremppi.report.pickData(tremppi.getItem('compared'), 'right');
        }
    },
    setPanel: function (panel) {
        tremppi.widget[panel] = $('#container_' + panel).w2grid(tremppi.widget.getGrid(panel));
    },
    findByName: function (list, name) {
        for (var i = 0; i < list.length; i++) {
            if (list[i].name === name) {
                return list[i];
            }
        }
        return {};
    },
    showPanel: function (name) {
        ['left', 'mid', 'right'].forEach(function (panel) {
            if (panel === name) {
                $('#container_' + panel).css('width', '100%').css('display', 'block');
            } else {
                $('#container_' + panel).css('display', 'none');
            }
        });
        tremppi.widget[name].resize();
    },
    showAll: function () {
        $('#container_left').css('width', '33.33%').css('display', 'block');
        $('#container_mid').css('width', '33.33%').css('display', 'block');
        $('#container_right').css('width', '33.33%').css('display', 'block');
        tremppi.widget.left.resize();
        tremppi.widget.mid.resize();
        tremppi.widget.right.resize();
    }
};


tremppi.log = function (content, level) {
    if (typeof level === 'undefined')
        level = 'info';

    var date = new Date();
    $('#log_line').html('[' + date.getHours() + ':' + date.getMinutes() + ':' + date.getSeconds() + '] ' + content);
    if ($('#log_line').length > 0)
        $('#log_line')[0].className = level;
};