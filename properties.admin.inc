<?php
// $Id$

/**
 * Menu callback; Display a list of categories.
 */
function properties_admin_categories_list() {
  return 'Categories list';
}

/**
 * Menu callback; Display a list of attributes.
 */
function properties_admin_attributes_list() {
  return 'Attributes list';
}

/**
 * Form builder; Add/Edit form for attributes.
 */
function properties_admin_attributes_form($form, &$form_state, $attribute = NULL) {
  if (empty($attribute)) {
    $attribute = (object)array(
      'label' => '',
      'name' => '',
    );
  }

  $form['label'] = array(
    '#title' => t('Attribute label'),
    '#type' => 'textfield',
    '#default_value' => $attribute->label,
    '#maxlength' => 255,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $attribute->name,
    '#maxlength' => 32,
    '#disabled' => !empty($type->name),
    '#machine_name' => array(
      'exists' => 'properties_attribute_load',
    ),
    '#description' => t('A unique machine-readable name for this attribute. It must only contain lowercase letters, numbers, and underscores. This name will be used when using this attribute.'),
  );

  $form['add'] = array(
    '#value' => t('Add'),
    '#type' => 'submit',
    '#submit' => array('properties_admin_attributes_add'),
  );

  $form['add_another'] = array(
    '#value' => t('Add another'),
    '#type' => 'submit',
    '#submit' => array('properties_admin_attributes_add_another'),
  );

  return $form;
}

/**
 * Form builder; Submit callback to save an attribute and redirect to list.
 */
function properties_admin_attributes_add($form, &$form_state) {
  properties_attribute_save((object)form_state_values_clean($form_state));
  $form_state['redirect'] = 'admin/config/content/properties/attributes';
}

/**
 * Form builder; Submit callback to save an attribute and stay on form.
 */
function properties_admin_attributes_add_another($form, &$form_state) {
  properties_attribute_save((object)form_state_values_clean($form_state));
}


/**
 * Form builder; Add/Edit form for categories.
 */
function properties_admin_categories_form($form, &$form_state, $category = NULL) {
  if (empty($category)) {
    $category = (object)array(
      'label' => '',
      'name' => '',
      'attributes' => array(),
    );
  }

  $form['label'] = array(
    '#title' => t('Category label'),
    '#type' => 'textfield',
    '#default_value' => $category->label,
    '#maxlength' => 255,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $category->name,
    '#maxlength' => 32,
    '#disabled' => !empty($type->name),
    '#machine_name' => array(
      'exists' => 'properties_category_load',
    ),
    '#description' => t('A unique machine-readable name for this category. It must only contain lowercase letters, numbers, and underscores. This name will be used when using this attribute.'),
  );

  $form['add'] = array(
    '#value' => t('Add'),
    '#type' => 'submit',
    '#submit' => array('properties_admin_categories_add'),
  );

  $form['add_another'] = array(
    '#value' => t('Add another'),
    '#type' => 'submit',
    '#submit' => array('properties_admin_categories_add_another'),
  );

  return $form;
}

/**
 * Form builder; Submit callback to save a category and redirect to list.
 */
function properties_admin_categories_add($form, &$form_state) {
  properties_categories_save((object)form_state_values_clean($form_state));
  $form_state['redirect'] = 'admin/config/content/properties/categories';
}

/**
 * Form builder; Submit callback to save a category and stay on form.
 */
function properties_admin_categories_add_another($form, &$form_state) {
  properties_categories_save((object)form_state_values_clean($form_state));
}
