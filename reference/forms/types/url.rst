.. index::
   single: Form; Campi; url

Tipo di campo url
=================

Il campo ``url`` è un campo testuale, che aggiunge al valore inserito un prefisso con
un protocollo dato (p.e. ``http://``), se il valore inserito non ha già tale
protocollo.

+---------------+-------------------------------------------------------------------+
| Reso come     | campo ``input url``                                               |
+---------------+-------------------------------------------------------------------+
| Opzioni       | - ``default_protocol``                                            |
+---------------+-------------------------------------------------------------------+
| Opzioni       | - `max_length`_                                                   |
| ereditate     | - `required`_                                                     |
|               | - `label`_                                                        |
|               | - `trim`_                                                         |
|               | - `read_only`_                                                    |
|               | - `disabled`_                                                     |
|               | - `error_bubbling`_                                               |
+---------------+-------------------------------------------------------------------+
| Tipo genitore | :doc:`text</reference/forms/types/text>`                          |
+---------------+-------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\UrlType` |
+---------------+-------------------------------------------------------------------+

Opzioni del campo
-----------------

default_protocol
~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``http``

Se un valore inserito non inizia per nessun protocollo (p.e. ``http://``,
``ftp://``, ecc.), questo protocollo sarà aggiunto come prefisso alla stringa, quando
i dati saranno legati al form.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
