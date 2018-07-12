
module.exports = function(RED) {
    "use strict";
    function AndinoX1Node(n) {
        RED.nodes.createNode(this,n);
        this.filter = n.filter || "events";
        var node = this;
        this.on("input", function (msg) {
            switch(node.filter ) {
                case "events": doEvents(node,msg);break;
                case "relay1": doRelais(node, msg, 1); break;
                case "relay2": doRelais(node, msg, 2); break;
            }
        });
    }

    function doEvents(node, msg) {

        var p = msg.payload.split('{');

	if( p.length != 3 )
	{
	  return;
	}

	var counter = p[1].replace('}','').split(',');
	var states  = p[2].replace('}','').split(',');

        var newMsg = {
            payload: {} };

	var i = 1;
	counter.forEach( function(c) {
	  newMsg.payload["Counter"+i] = parseInt("0x"+c);
    	  i++;
	});

	i = 1;
	states.forEach( function(p) {
      	  newMsg.payload["Pin"+i] = parseInt("0x"+p);
          i++;
    	});

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
