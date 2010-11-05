<?php

$element = $variables['element'];
$output = '';

$table_id = 'properties-table';

$header = array(
  t('Attribute name'),
  t('Attribute label'),
  t('Value'),
  t('Category'),
  t('Order'),
);
$rows = array();

drupal_add_tabledrag($table_id, 'match', 'parent', 'property-category-select', 'property-category-select', 'category-name');
drupal_add_tabledrag($table_id, 'order', 'sibling', 'property-weight');

foreach (element_children($element['listing']) as $category_name) {
  $category = $element['listing'][$category_name];
  $category['_weight']['#attributes']['class'] = array('property-weight');
  $category['name']['#attributes']['class'] = array('category-name');
  $category['category']['#attributes']['class'] = array('property-category-select');

  $cells = array(
    'data' => array(
      $category['#label'],
      '',
      '',
      '',
      drupal_render($category['category']) . drupal_render($category['name']) . drupal_render($category['_weight']),
    ),
    'class' => array('draggable', 'tabledrag-root'),
  );
  $rows[] = $cells;

  foreach (element_children($category['properties']) as $delta) {
    $property = $category['properties'][$delta];

    // Set special classes needed for table drag and drop.
    $property['category']['#attributes']['class'] = array('property-category-select');
    $property['_weight']['#attributes']['class'] = array('property-weight');

    $cells = array(
      array('data' => theme('indentation', array('size' => 1)) . drupal_render($property['attribute']), 'class' => array('properties-attribute-row-small')),
      drupal_render($property['label']),
      drupal_render($property['value']),
      drupal_render($property['category']),
      drupal_render($property['_weight']),
    );
    $rows[] = array(
      'data' => $cells,
      'class' => array('draggable', 'tabledrag-leaf'),
    );
  }
}

$output = '<div class="form-item">';
$output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('properties-table'))));
$output .= '<div class="clearfix inline">' . drupal_render($element['new_category']) . drupal_render($element['add_category']) . '</div>';
$output .= '<div class="clearfix inline">' . drupal_render($element['attribute_category']) . drupal_render($element['new_attribute']) . drupal_render($element['add_attribute']) . '</div>';
$output .= '</div>';

echo $output;
