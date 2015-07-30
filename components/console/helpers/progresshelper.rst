.. index::
    single: Aiutanti di Console; Aiutante Progress

Aiutante Progress
=================

.. versionadded:: 2.2
    L'aiutante ``progress`` è stato aggiunto in Symfony 2.2.

.. versionadded:: 2.3
    Il metodo ``setCurrent`` è stato aggiunto in Symfony 2.3.

Quando si eseguono comandi che devono girare a lungo, può essere utile mostrare una barra di progresso,
che si aggiorna durante l'esecuzione:

.. image:: /images/components/console/progress.png

Per mostrare dettagli sul progresso, usare la classe :class:`Symfony\\Component\\Console\\Helper\\ProgressHelper`,
passargli un numero totale di unità e avanzare il progresso man mano che il comando gira::

    $progress = $this->getHelperSet()->get('progress');

    $progress->start($output, 50);
    $i = 0;
    while ($i++ < 50) {
        // ... fare qualcosa

        // avanzare la barra di progresso di 1 unità
        $progress->advance();
    }

    $progress->finish();

.. tip::

    Si può anche impostare il progresso attuale, richiamando il metodo
    :method:`Symfony\\Component\\Console\\Helper\\ProgressHelper::setCurrent`.


Si può anche personalizzare il modo in cui il progresso viene mostrato, con vari
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
