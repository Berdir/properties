(function ($) {
  Drupal.behaviors.propertiesUpdateTabIndex = {
    attach: function (context) {

        $(document).bind('mouseup', function (event) {
            var tabindex = 1;
            $('table.properties-table input.properties-value').each(function(context) {
                $(this).attr('tabindex', tabindex++);
            })
        });
    }
  }
})(jQuery);


