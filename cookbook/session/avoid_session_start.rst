.. index::
    single: Sessioni, cookie

Evitare di far partire le sessioni per utenti anonimi
=====================================================

Le sessioni partono automaticamente ogni volta che si legge, scrive o anche solo verifica
l'esistenza di dati in sessione. Questo vuole dire che, se si deve evitare di creare
un cookie di sessione per alcuni utenti, potrebbe risultare difficile: si deve evitare *completamente*
l'accesso alla sessione.

Per esempio, un tipico problema in questa situazione è la verifica di messaggi flash,
che sono memorizzati in sessione. Il codice seguente può garantire che
una sessione sia *sempre* partita:

.. code-block:: html+jinja

    {% for flashMessage in app.session.flashbag.get('notice') %}
        <div class="flash-notice">
            {{ flashMessage }}
        </div>
    {% endfor %}

Anche se l'utente non è connesso e anche se non sono stati creati messaggi flash,
basta richiamare il metodo ``get()`` (o anche ``has()``) su ``flashbag`` per far
partire una sessione. Questo potrebbe influire sulle prestazioni di un'applicazione, perché tutti gli
utenti riceverebbero un cookie di sessione. Per evitare tale comportamento, aggiungere un controllo prima
di provare ad accedere ai messaggi flash:

.. code-block:: html+jinja

    {% if app.request.hasPreviousSession %}
        {% for flashMessage in app.session.flashbag.get('notice') %}
            <div class="flash-notice">
                {{ flashMessage }}
            </div>
        {% endfor %}
    {% endif %}
