.. index::
   single: Test

Come testare l'interazione con diversi client
=============================================

Se occorre simulare un'interazionoe tra diversi client (si pensi a una chat,
per esempio), creare tanti client::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

Questo non funziona se il proprio codice mantiene uno stato globale o se dipende da
librerie di terze parti che abbiano un qualche tipo di stato globale. In questo caso,
si possono isolare i client::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->insulate();
    $sally->insulate();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

Client isolati possono eseguire trasparentemente le loro richieste in un processo PHP
dedicato e pulito, evitando quindi effetti collaterali.

.. tip::

    Essendo un client isolato più lento, si può mantenere un client nel processo
    principale e isolare gli altri.
