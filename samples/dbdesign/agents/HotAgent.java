
import lotus.notes.*;

/* Java Agent that opens the database "problems.nsf" and
 * updates the Support Rep field of any documents with an Open 
 * Status and High Priority.
 */

public class HotAgent extends AgentBase
{

    Session mySess;
    AgentContext myAC;
    Database myDB;
    Document myDoc;
    DocumentCollection myColl;
    String repName = "Fire Fighters";
    String valStatus;
    String valPriority;
    
    public void NotesMain()
    {
        int i;
        
        try
        {
                /* create a new session */
                mySess = Session.newInstance();

                /* set database object to problems.nsf */
                myDB = mySess.getDatabase("","PROBLEMS.NSF");
       
                /* get all the documents in the database */
                myColl = myDB.getAllDocuments();

                /* for each document get the Status and Priority Values.
                 * If their Open and High set the Support Rep name to
                 * "Fire Fighters" and save the change.
                */
                for (i = 1;i <= myColl.getCount(); i++)
                {
                   myDoc = myColl.getNthDocument(i);
                   valStatus = myDoc.getItemValueString("Status");
                   valPriority = myDoc.getItemValueString("Priority");
                   if (valStatus.equals("Open") && valPriority.equals("High"))
                   {
                      myDoc.replaceItemValue("SRep",repName);
                      myDoc.save(false,true);
                   }
                }
         }
         catch(Exception e)
         {
                 e.printStackTrace();
         }
              
    }
}

