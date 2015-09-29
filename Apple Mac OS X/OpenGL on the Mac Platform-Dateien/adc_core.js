Event.onDOMReady(function() {
	var container = $(document.body);
	container.addClassName('hasjs');
});

Event.onDOMReady(function() {
	var tables = $$('table.stroke');
	tables.each(function(table) {
		var select = function(selector1, selector2) {
			var cells = table.select(selector1);
			cells = cells.concat(table.select(selector2));
			return cells;
		}.bind(table);

		var cells = select('tr:first-child th', 'tr:first-child td');
		cells.each(function(cell) { cell.style.borderTop = 'none' });

		var cells = select('tr:last-child th', 'tr:last-child td');
		cells.each(function(cell) { cell.style.borderBottom = 'none' });

		var cells = select('tr th:first-child', 'tr td:first-child');
		cells.each(function(cell) { cell.style.borderLeft = 'none' });

		var cells = select('tr th:last-child', 'tr td:last-child');
		cells.each(function(cell) { cell.style.borderRight = 'none' });
	});
});



// 
// ADC Core
// 

if (typeof(ADC) == "undefined") ADC = {};

ADC.cancelEvent = function(evt) {
	if (evt) {
		var link = evt.findElement('a');
		if (link) {
			if (!link.href.match('#')) {
				evt.stop();
			} else if (!AC.Detector.isWebKit() || (AC.Detector.isWebKit() && parseInt(AC.Detector.getAgent().match(/version\/(\d)/)[1])>2)) {
				evt.stop();
				document.location.hash = link.href.replace(/.*#/, '');
			}
		}
	}
}

ADC.Menus = Class.create({
	menus: [],

	initialize: function(menus, event) {
		this.setMenus(menus, event);
	},

	setMenus: function(menus, event) {
		if (!event) var event = 'click';

		menus.each(function(menu) {
			var menu = new ADC.Menu(menu, event);
			this.setEvent(menu, event);
			this.menus.push(menu);
		}.bind(this));
	},

	setEvent: function(menu, event) {
		Event.observe(menu.menu, event, function(evt) {
			ADC.cancelEvent(evt);
			var menus = this.menus.without(menu);
			menus.each(function(menu) {
				menu.close();
			});
		}.bind(this), false);
	}
});

ADC.Menu = Class.create({
	initialize: function(menu, event) {
		this.menu = menu;
		this.setEvents(event);
	},

	setEvents: function(event) {
		if (!event) var event = 'click';
		
		if (event == 'click') this.setHovers();

		Event.observe(this.menu, event, this.toggleMenu.bind(this), false);
	},

	setHovers: function(event) {
		Event.observe(this.menu, 'mouseover', function() {
			Element.addClassName(this.menu, 'hover');
		}.bind(this), false);

		Event.observe(this.menu, 'mouseout', function() {
			Element.removeClassName(this.menu, 'hover');
		}.bind(this), false);
	},
	
	linkWithin: function(evt) {
		var link = evt.findElement('a');
		if (link.up().up().up() == this.menu) {
			document.location = link.href;
			return true;
		}
		
		return false;
	},

	toggleMenu: function(evt) {
		if (!this.linkWithin(evt)) ADC.cancelEvent(evt);

		if (!this.menu.hasClassName('active')) {
			this.open();
		} else {
			this.close();
		}
	},

	open: function() {
		this.menu.addClassName('active');
	},

	close: function() {
		this.menu.removeClassName('active');
	}

});
