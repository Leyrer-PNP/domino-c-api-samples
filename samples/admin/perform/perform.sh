echo Copying empty Name and Address book to Notes data directory...
cp  $LOTUS/notesapi/notedata/na_test.nsf  $NOTES_DATA_DIR

echo Loading database with initial set of names...
echo addnames  na_test  50
addnames  na_test  50
echo
echo

echo Indexing the new names...
echo index  na_test  Contacts
index  na_test  Contacts
echo
echo

echo Performing mixed transactions with optimal indexing...
echo mixed_1  na_test  40
mixed_1  na_test  40
echo
echo

echo Performing mixed transactions with worst-case indexing...

echo mixed_2  na_test  40
mixed_2  na_test  40
echo
echo
echo Done
