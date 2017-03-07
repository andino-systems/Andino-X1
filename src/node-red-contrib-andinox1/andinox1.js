
module.exports = function(RED) {
    "use strict";
    function AndinoX1Node(n) {
        RED.nodes.createNode(this,n);
        this.filter = n.filter || "counter";
        var node = this;
        this.on("input", function (msg) {
            switch(node.filter ) {
                case "counter": doCounter(node,msg);break;
                case "relais1": doRelais(node, msg, 1); break;
                case "relais2": doRelais(node, msg, 2); break;
                case "event": doEvent(node, msg); break;
            }
        });
    }

    function doCounter(node, msg) {
        if (msg.payload.indexOf("CNTR") < 0) {
            return;
        }
        var parts = msg.payload.split(' ');
        if (parts.length != 4) {
            node.log("invalid message received. has not 4 parts");
            return;
        }
        var newMsg = {
            payload: { counter1: Number(parts[2]), counter2: Number(parts[3]) } }
        node.send(newMsg);
    }

    function doEvent(node, msg) {
        if (msg.payload.indexOf("STAT") < 0) {
            return;
        }
        var parts = msg.payload.split(' ');
        if (parts.length != 4) {
            node.log("invalid message received. has not 3 parts");
            return;
        }
        var newMsg = {
            payload: { pin1: Number(parts[2]), pin2: Number(parts[3]) }
        }
        node.send(newMsg);
    }

    function doRelais(node, msg, relais) {
        var state = toBoolean(msg.payload);
        if (state == null)
            return;
        var newMsg =
        {
            payload: "REL" + relais + " " + ((state) ? "1" : "0") + "\r\n"
        }
        node.send(newMsg);
    }

    function toBoolean(source){
        if (typeof(source) == "boolean") {
            return source;
        }
        if (typeof(source) == "string") {
            switch (source.toLowerCase().trim()) {
            case "true":
            case "yes":
            case "1":
                return true;
            case "false":
            case "no":
            case "0":
                return false;
            default:
                return null;
            }
        }
        return null;
    }

    RED.nodes.registerType("AndinoX1", AndinoX1Node);
}
