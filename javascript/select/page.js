/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

tremppi.select.setPage = function () {
    $("#widget").append('<div id="selection_table"></div>');
};

tremppi.select.setData = function () {
    tremppi.select.setColumns();
    tremppi.select.loadTable();
};

tremppi.select.setDefaultData = function () {
    tremppi.data.name = "grid";
    if (typeof tremppi.data.columns === 'undefined')
        tremppi.data.columns = [];
     
    if (typeof tremppi.data.records === 'undefined' || tremppi.data.records.length === 0)
        tremppi.data.records = [{recid: 0}];
};