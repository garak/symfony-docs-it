.. code-block:: php

    interface VoterInterface
    {
        public function supportsAttribute($attribute);
        public function supportsClass($class);
        public function vote(TokenInterface $token, $object, array $attributes);
    }

Il metodo :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::supportsAttribute`
è usato per verificare che il votante supporti l'attributo utente dato (p.e.:
un ruolo come ``ROLE_USER``, un'ACL come ``EDIT``, ecc.)

Il metodo :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::supportsClass`
è usato per verificare che il votante supporti l'attuale
classe per il token dell'utente.

Il metodo :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::vote`
deve implementare la logica di business che verifica se l'utente possa
avere accesso o meno. Questo metodo deve restituire uno dei seguenti valori:

* ``VoterInterface::ACCESS_GRANTED``: L'utente può accedere all'applicazione
* ``VoterInterface::ACCESS_ABSTAIN``: Il votante non può decidere se l'utente possa accedere o meno
* ``VoterInterface::ACCESS_DENIED``: L'utente non può accedere all'applicazione
