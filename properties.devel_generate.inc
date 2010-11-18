<?php

/**
 * Implements hook_devel_generate().
 */
function properties_devel_generate($object, $field, $instance, $bundle) {
  return array(
    0 => array(
      'screen' => array(
        '_weight' => 1,
        'properties' => array(
          'screen_x' => array(
            'category' => 'screen',
            'attribute' => 'screen_x',
            'value' => devel_generate_word(10),
          ),
        ),
      ),
    ),
  );
}
