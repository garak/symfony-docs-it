.. versionadded:: 2.3
    L'opzione ``action`` è stata introdotta in Symfony 2.3.

action
~~~~~~

**tipo**: ``stringa`` **predefinito**: stringa vuota

Questa opzione specifica dove inviare i dati del form (solitamente un
URI). Il suo valore viene reso come attributo ``action`` dell'elemento ``form``.
Un valore vuoto viene considerato come riferimento allo stesso documento, cioè il form
sarà inviato allo stesso URI che ha reso il form.
