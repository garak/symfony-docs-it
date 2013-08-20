.. versionadded:: 2.1
    L'opzione ``error_mapping`` è nuova in Symfony 2.1.

error_mapping
~~~~~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``empty``

Questa opzione consente di modificare il bersaglio di un errore di validazione.

Si immagini di avere un metodo personalizzato, di nome ``matchingCityAndZipCode``, che valida
se una città e un codice postale corrsispondano. Purtroppo non c'è un campo "matchingCityAndZipCode"
nel form, quindi tutto ciò che può fare Symfony è mostrare l'errore in cima
al form stesso.

Con la mappatura personalizzata degli errori, si può fare meglio: mappare l'errore al campo
della città, in modo che sia mostrato accanto a esso::

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'error_mapping' => array(
                'matchingCityAndZipCode' => 'city',
            ),
        ));
    }

Ecco le regole per i lati destro e sinistro della mappatura:

* I lato sinistro contiene i percorsi delle proprietà.
* Se la validazione è generata su una proprietà o metodo di una classe, il suo percorso
  è semplicemente "nomeProprietà".
* Se la validazione è generata su un elemento di un ``array`` od oggetto ``ArrayAccess``,
  il percorso è ``[nomeIndice]``.
* Si possono costruire percorsi innestati, concatenandoli, separando le
  proprietà con dei punti. Per esempio: ``addresses[work].matchingCityAndZipCode``
* Anche il lato sinistro della mappatura accetta un punto ``.``, che si riferisce
  al campo stesso. Questo significa che un errore aggiunto al campo è aggiunto
  invece al dato campo innnestato.
* Il lato destro contiene semplicemente i nomi dei campi nel form.













