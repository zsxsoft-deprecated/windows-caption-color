function getARGB(object) {
	var HSV = object.hsv;
	var A = Math.round(parseInt(HSV.substr(0, 2), 16) / 2.55) / 100;
	var R1 = parseInt(HSV.substr(2, 2), 16);
	var G1 = parseInt(HSV.substr(4, 2), 16);
	var B1 = parseInt(HSV.substr(6, 2), 16);
	object.a = A;
	object.r = R1;
	object.g = G1;
	object.b = B1;
}

function get(callback) {
	var addon = require('bindings')('./color.node');
	var Winreg = require('winreg');
	try {
		var color = addon.color();
	} catch (e) {
		callback(false);
		return;
	}

	if (!color) {
		callback(false);
		return;
	}

	var HSV = color.color.substr(0, 8); // TODO: fix error response.
	var regKey = new Winreg({
		hive: Winreg.HKCU,
		key: '\\Software\\Microsoft\\Windows\\DWM'
	});
	var ret = {
		dwm: {
			hsv: HSV,
			r: 0,
			a: 0,
			g, 0,
			b: 0,
			opaque: color.opaque
		},
		reg: {
			hsv: "0000000",
			r: 0,
			a: 0,
			g, 0,
			b: 0,
			balance: 1
		}
	};
	getARGB(ret.dwm);
	regKey.values(function (err, items) {
		if (err) { // Not support Windows 7
			callback(null, ret);
			return;
		}
		items.forEach(function (item) {
			var lowerKey = item.name.toLowerCase();
			if (lowerKey == "colorizationcolor") {
				ret.reg.hsv = item.value.replace("0x", "");
			} else if (lowerKey == "colorizationcolorbalance") {
				ret.reg.balance = parseInt(item.value.replace("0x", ""), 10) / 100;
			}
		});
		getARGB(ret.reg);
		callback(null, ret);
	});

	return true;
}
module.exports = {
	get: get
};