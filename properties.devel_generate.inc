<?php
// $Id$

/**
 * @file
 * Contains devel_generate hook implementation to fill properties fields.
 */

/**
 * Implements hook_devel_generate().
 */
function properties_devel_generate($object, $field, $instance, $bundle) {
  static $categories = NULL;
  if (!isset($categories)) {
    $categories = properties_category_load_multiple(array_keys(properties_category_load_paging(50)));
  }

  $items = array();
  $max = rand(0, min(array(count($categories), 5)));
  $cweight = 0;
  for ($i = 0; $i < $max; $i++) {
    // Make sure the category is not yet selected.
    do {
      $category = $categories[array_rand($categories)];
    } while (isset($items[$category->name]));

    $items[$category->name] = array(
      '_weight' => $cweight++,
    );

    $aweight = 0;
    foreach ($category->attributes as $attribute) {
      $items[$category->name]['properties'][$attribute->name] = array(
        'category' => $category->name,
        'attribute' => $attribute->name,
        'value' => devel_generate_word(10),
        '_weight' => $aweight++,
      );
    }
  }

  if (count($items)) {
    return array(0 => $items);
  }
}
