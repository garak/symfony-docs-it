.. index::
    single: Aiutanti di console; Tabella

Tabella
=======

.. versionadded:: 2.5
    La classe ``Table`` è stata introdotta in Symfony 2.5 come rimpiazzo per
    :doc:`l'aiutante Table </components/console/helpers/tablehelper>`.

Può essere utile mostrare dati in tabella in un'applicazione di console:

.. code-block:: text

    +---------------+--------------------------+------------------+
    | ISBN          | Titolo                   | Autore           |
    +---------------+--------------------------+------------------+
    | 99921-58-10-7 | La divina commedia       | Dante Alighieri  |
    | 9971-5-0210-0 | A Tale of Two Cities     | Charles Dickens  |
    | 960-425-059-0 | The Lord of the Rings    | J. R. R. Tolkien |
    | 80-902734-1-6 | And Then There Were None | Agatha Christie  |
    +---------------+--------------------------+------------------+

Per mostrare una tabella, usare :class:`Symfony\\Component\\Console\\Helper\\Table`,
impostare i titoli, impostare le righe e quindi rendere la tabella::

    use Symfony\Component\Console\Helper\Table;

    $table = new Table($output);
    $table
        ->setHeaders(array('ISBN', 'Titolo', 'Autore'))
        ->setRows(array(
            array('99921-58-10-7', 'La divina commedia', 'Dante Alighieri'),
            array('9971-5-0210-0', 'A Tale of Two Cities', 'Charles Dickens'),
            array('960-425-059-0', 'The Lord of the Rings', 'J. R. R. Tolkien'),
            array('80-902734-1-6', 'And Then There Were None', 'Agatha Christie'),
        ))
    ;
    $table->render();

Si può aggiumgere un separatore in un punto dell'output, passando un'istanza di
:class:`Symfony\\Component\\Console\\Helper\\TableSeparator` come riga::

    use Symfony\Component\Console\Helper\TableSeparator;

    $table->setRows(array(
        array('99921-58-10-7', 'La divina commedia', 'Dante Alighieri'),
        array('9971-5-0210-0', 'A Tale of Two Cities', 'Charles Dickens'),
        new TableSeparator(),
        array('960-425-059-0', 'The Lord of the Rings', 'J. R. R. Tolkien'),
        array('80-902734-1-6', 'And Then There Were None', 'Agatha Christie'),
    ));

.. code-block:: text

    +---------------+--------------------------+------------------+
    | ISBN          | Titolo                   | Autore           |
    +---------------+--------------------------+------------------+
    | 99921-58-10-7 | La divina commedia       | Dante Alighieri  |
    | 9971-5-0210-0 | A Tale of Two Cities     | Charles Dickens  |
    +---------------+--------------------------+------------------+
    | 960-425-059-0 | The Lord of the Rings    | J. R. R. Tolkien |
    | 80-902734-1-6 | And Then There Were None | Agatha Christie  |
    +---------------+--------------------------+------------------+

Si può cambiare stile, usando uno di quelli disponibili, tramite
:method:`Symfony\\Component\\Console\\Helper\\Table::setStyle`::

    // lo stesso che non impostare niente
    $table->setStyle('default');

    // cambia in stile compatto
    $table->setStyle('compact');
    $table->render();

Il risultato è:

.. code-block:: text

     ISBN          Titolo                   Autore
     99921-58-10-7 La divina commedia       Dante Alighieri
     9971-5-0210-0 A Tale of Two Cities     Charles Dickens
     960-425-059-0 The Lord of the Rings    J. R. R. Tolkien
     80-902734-1-6 And Then There Were None Agatha Christie

Si può anche impostare lo stile a ``borderless``::

    $table->setStyle('borderless');
    $table->render();

che mostra:

.. code-block:: text

     =============== ========================== ==================
      ISBN            Titolo                    Autore
     =============== ========================== ==================
      99921-58-10-7   La divina commedia         Dante Alighieri
      9971-5-0210-0   A Tale of Two Cities       Charles Dickens
      960-425-059-0   The Lord of the Rings      J. R. R. Tolkien
      80-902734-1-6   And Then There Were None   Agatha Christie
     =============== ========================== ==================

Se nessuno degli stili predefiniti è adatto, se ne può definire uno nuovo::

    use Symfony\Component\Console\Helper\TableStyle;

    // basato su "default", se non specificato diversamente 
    $style = new TableStyle();

    // personalizzare lo stile
    $style
        ->setHorizontalBorderChar('<fg=magenta>|</>')
        ->setVerticalBorderChar('<fg=magenta>-</>')
        ->setCrossingChar(' ')
    ;

    // usare lo stile
    $table->setStyle($style);

Ecco una lista delle cose che si possono personalizzare:

*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setPaddingChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setHorizontalBorderChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setVerticalBorderChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setCrossingChar`
*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setCellHeaderFormat`
*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setCellRowFormat`
*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setBorderFormat`
*  :method:`Symfony\\Component\\Console\\Helper\\TableStyle::setPadType`

.. tip::

    Si può anche registrare uno stile globalmente::

        // registra lo stile con nome "colorato"
        Table::setStyleDefinition('colorato', $style);

        // usa lo stile
        $table->setStyle('colorato');

    Si può usare lo steso metodo per ridefinire uno degli stili predefiniti.
