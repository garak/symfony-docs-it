.. index::
    single: Aiutanti di Console; Aiutante Progress

Aiutante Progress
=================

.. versionadded:: 2.3
    Il metodo ``setCurrent`` è stato introdotto in Symfony 2.3.

.. versionadded:: 2.4
    Il metodo  ``clear`` è stato introdotto in Symfony 2.4.

.. caution::

    L'aiutante Progress è stato deprecato in Symfony 2.5 e sarà rimosso in
    Symfony 3.0. Si dovrebbe invece usare la
    :doc:`barra di progressione </components/console/helpers/progressbar>`, che
    è più potente.

Quando si eseguono comandi che devono girare a lungo, può essere utile mostrare una barra di progressione,
che si aggiorna durante l'esecuzione:

.. image:: /images/components/console/progress.png

Per mostrare dettagli sulla progressione, usare la classe :class:`Symfony\\Component\\Console\\Helper\\ProgressHelper`,
passando il numero totale di unità e avanzando la progressione man mano che il comando gira::

    $progress = $this->getHelperSet()->get('progress');

    $progress->start($output, 50);
    $i = 0;
    while ($i++ < 50) {
        // ... fare qualcosa

        // avanzare la barra di progressione di 1 unità
        $progress->advance();
    }

    $progress->finish();

.. tip::

    Si può anche impostare la progressione attaule, richiamando il metodo
    :method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::setCurrent`.


Se si vuole mostrare qualcosa mentre la barra di progressione avanza,
richiamare prima :method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::clear`.
Dopo aver finito, richiamre
:method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::display`
per mostrare di nuovo la barra di progressione.

Si può anche personalizzare il modo in cui la progressione viene mostrata, con vari
livelli di verbosità. Ciascuno di questi mostra diversi possibili
elementi, come la percentuale di completamento, una barra mobile, o informazioni
su attuale/totale (p.e. 10/50)::

    $progress->setFormat(ProgressHelper::FORMAT_QUIET);
    $progress->setFormat(ProgressHelper::FORMAT_NORMAL);
    $progress->setFormat(ProgressHelper::FORMAT_VERBOSE);
    $progress->setFormat(ProgressHelper::FORMAT_QUIET_NOMAX);
    // valore predefinito
    $progress->setFormat(ProgressHelper::FORMAT_NORMAL_NOMAX);
    $progress->setFormat(ProgressHelper::FORMAT_VERBOSE_NOMAX);

Si possono anche controllare i vari caratteri e la larghezza usata per
la barra::

    // la parte finale della barra
    $progress->setBarCharacter('<comment>=</comment>');
    // la parte in corso della barra
    $progress->setEmptyBarCharacter(' ');
    $progress->setProgressCharacter('|');
    $progress->setBarWidth(50);

Per tutte le opzioni disponibili, consultare la documentazione delle API di
:class:`Symfony\\Component\\Console\\Helper\\ProgressHelper`.

.. caution::

    Per questioni prestazionali, fare attenzione a non impostare un numero totale di passi
    troppo elevato. Per esempio, se si itera un gran numero
    di elementi, considerare l'impostazione di una frequenza maggiore, richiamando
    :method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::setRedrawFrequency`,
    in modo da aggiornare solamente ogni tot iterazioni::

        $progress->start($output, 50000);

        // avanzare ogni 100 iterazioni
        $progress->setRedrawFrequency(100);

        $i = 0;
        while ($i++ < 50000) {
            // ... fare qualcosa

            $progress->advance();
        }
