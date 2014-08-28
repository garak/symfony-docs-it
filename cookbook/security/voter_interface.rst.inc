.. code-block:: php

    interface VoterInterface
    {
        public function supportsAttribute($attribute);
        public function supportsClass($class);
        public function vote(TokenInterface $token, $object, array $attributes);
    }

Il metodo :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::supportsAttribute`
viene usato per verificare se il votante supporta il dato attributo (quindi
un ruolo come ``ROLE_USER``, un ACL ``EDIT``, ecc.).

Il metodo :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::supportsClass`
viene usato per verificare se il votante supporta la classe dell'oggetto il
cui accesso è in corso di verifica.

Il metodo :method:`Symfony\\Component\\Security\\Core\\Authorization\\Voter\\VoterInterface::vote`
deve implementare la logica di business che verifica se l'utente abbia accesso
o meno. Tale metodo deve restituire uno dei seguenti valori:

* ``VoterInterface::ACCESS_GRANTED``: il votante concede l'autorizzazione;
* ``VoterInterface::ACCESS_ABSTAIN``: il votante non può decidere se concedere o meno l'autorizzazione;
* ``VoterInterface::ACCESS_DENIED``: il votante nega l'autorizzazione.
