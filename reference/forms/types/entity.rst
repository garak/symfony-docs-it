.. index::
   single: Form; Campi; choice

Tipo di campo entity
====================

Un campo speciale ``choice`` pensato per caricare opzioni da un'entità Doctrine.
Per esempio, se si ha un'entità ``Category``, si può usare questo campo
per mostrare un campo ``select`` di tutti, o di alcuni, oggetti ``Category``
presi dalla base dati.

+---------------+---------------------------------------------------------------------+
| Reso come     | possono essere vari tag (vedere :ref:`forms-reference-choice-tags`) |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `class`_                                                          |
|               | - `data_class`_                                                     |
|               | - `em`_                                                             |
|               | - `group_by`_                                                       |
|               | - `property`_                                                       |
|               | - `query_builder`_                                                  |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `choices`_                                                        |
| ridefinite    | - `choice_list`_                                                    |
+---------------+---------------------------------------------------------------------+
| Opzioni       | dal tipo :doc:`choice </reference/forms/types/choice>`:             |
| ereditate     |                                                                     |
|               | - `empty_value`_                                                    |
|               | - `expanded`_                                                       |
|               | - `multiple`_                                                       |
|               | - `preferred_choices`_                                              |
|               |                                                                     |
|               | dal tipo :doc:`form </reference/forms/types/form>`:                 |
|               |                                                                     |
|               | - `data`_                                                           |
|               | - `disabled`_                                                       |
|               | - `empty_data`_                                                     |
|               | - `error_bubbling`_                                                 |
|               | - `error_mapping`_                                                  |
|               | - `label`_                                                          |
|               | - `label_attr`_                                                     |
|               | - `mapped`_                                                         |
|               | - `read_only`_                                                      |
|               | - `required`_                                                       |
+---------------+---------------------------------------------------------------------+
| Tipo genitore | :doc:`choice </reference/forms/types/choice>`                       |
+---------------+---------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Bridge\\Doctrine\\Form\\Type\\EntityType`          |
+---------------+---------------------------------------------------------------------+

Utilizzo di base
----------------

Il tipo ``entity`` ha una sola opzione obbligatoria: l'entità da elencare all'interno
del campo choice::

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'property' => 'username',
    ));

In questo caso, tutti gli oggetti ``User`` saranno caricati dalla base dati e resi come
un tag ``select``, dei radio o una serie di checkbox (a seconda dei valori di
``multiple`` ed ``expanded``).
Se l'oggetto entità manca del metodo ``__toString()``, occorre passare l'opzione
``property``.

Usare una query personalizzata per le entità
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se occorre specificare una query personalizzata da usare per recuperare le entità
(p.e. si vogliono solo alcune entità o le si vuole ordinare), usare l'opzione
``query_builder``. Il modo più facile è fare come segue::

    use Doctrine\ORM\EntityRepository;
    // ...

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'query_builder' => function (EntityRepository $er) {
            return $er->createQueryBuilder('u')
                ->orderBy('u.username', 'ASC');
        },
    ));

.. _reference-forms-entity-choices:

Usare Choices
~~~~~~~~~~~~~

Se si dispone già della collezione di entità che si vuole includere
nell'elemento choice, si può semplicemente passarla tramite l'opzione ``choices``.
Per esempio, se si ha una variabile ``$group`` (magari passata
come opzione del form) e ``getUsers`` restitusice una collezione di entità ``User``,
si può fornire direttamente l'opzione ``choices``::

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'choices' => $group->getUsers(),
    ));

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Opzioni del campo
-----------------

class
~~~~~

**tipo**: ``stringa`` **required**

La classe dell'entità (p.e. ``AcmeStoreBundle:Category``). Può essere
un nome di classe pienamente qualificato (p.e. ``Acme\StoreBundle\Entity\Category``)
o il suo alias (come mostrato sopra).

.. include:: /reference/forms/types/options/data_class.rst.inc

em
~~

**tipo**: ``stringa`` **predefinito**: il gestore di entità predefinito

Se specificato, il gestore di entità da usare per caricare le scelte, al posto
di quello predefinito.

group_by
~~~~~~~~

**tipo**: ``stringa``

Il percorso di proprietà (p.e. ``author.name``) usato per organizzare le scelte
disponibili in gruppi. Funziona solo se reso come tag select e lo fa
aggiungendo tag optgroup tra le opzioni. Le scelte che non restituiscono un
valore per questo percorso di proprietà sono rese direttamente sotto il tag
select, senza optgroup.

property
~~~~~~~~

**tipo**: ``stringa``

La proprietà da usare per mostrare le entità come testi nell'elemento HTML.
Se lasciata vuota, gli oggetti saranno formattati come stringhe, quindi
occorre avere un metodo ``__toString()``.

.. note::

    L'opzione ``property`` è il percorso della proprietà usato per mostrare l'opzione.
    Si può quindi usare qualsiasi cosa supportata dal
    :doc:`componente PropertyAccessor </components/property_access/introduction>`

    Per esempio, se la proprietà translations è un array associativo di
    oggetti, ciascuno con una proprietà name, si potrebbe fare così::

        $builder->add('gender', 'entity', array(
           'class' => 'MyBundle:Gender',
           'property' => 'translations[en].name',
        ));

query_builder
~~~~~~~~~~~~~

**tipo**: ``Doctrine\ORM\QueryBuilder`` oppure una closure

Se specificato, è usato per cercare un sotto-insieme di opzioni (e il loro
ordinamento) che dovrebbero essere usate per il campo. Il valore di questa opzione
può essere un oggetto ``QueryBuilder`` oppure una closure. Se su usa una closure,
dovrebbe accettare un singolo parametro, che è l'``EntityRepository``
dell'entità.

Opzioni ridefinite
------------------

choice_list
~~~~~~~~~~~

**predefinito**: :class:`Symfony\\Bridge\\Doctrine\\Form\\ChoiceList\\EntityChoiceList`

Lo scopo del tipo ``entity`` è creare e configurare ``EntityChoiceList``,
usando una delle opzioni precedenti. Se occorre sovrascrivere questa
opzione, si può prendere in considerazione l'uso diretto di :doc:`/reference/forms/types/choice`.


choices
~~~~~~~

**tipo**:  array || ``\Traversable`` **predefinito**: ``null``

Invece di lasciare che `class`_ e `query_builder`_ recuperino le
entità da inserire, si può passare direttamente l'opzione ``choices``.
Vedere :ref:`reference-forms-entity-choices`.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice </reference/forms/types/choice>`:


.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/multiple.rst.inc

.. note::

    Se si ha a che fare con una collezione di entità Doctrine, sarà utile
    leggere la documenzione anche di
    :doc:`/reference/forms/types/collection`. In aggiunta, c'è
    un esempio completo nella ricetta
    :doc:`/cookbook/form/form_collections`.

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. note::

    Questa opzione si aspetta un array di oggetti entità, diversamente dal campo ``choice``,
    che richiede un array di chiavi.

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:


.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :end-before: DEFAULT_PLACEHOLDER

Il valore predefinito effettivo di questa opzione dipende da altre opzioni:

* Se ``multiple`` è ``false`` ed ``expanded`` è ``false``, allora ``''``
  (stringa vuota);
* Altrimenti ``array()`` (array vuoto).

.. include:: /reference/forms/types/options/empty_data.rst.inc
    :start-after: DEFAULT_PLACEHOLDER

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc
