import lotus.domino.*;

public class IntroLocalApplet extends AppletBase
{
   java.awt.TextArea ta;

   public void notesAppletInit()
   {
	  setLayout(null);
	  setSize(462,120);
	  ta = new java.awt.TextArea();
	  ta.setBounds(0,0,460,118);
	  add(ta);
	  ta.setEditable(false);
	  setVisible(true);
   }   
   public void notesAppletStart()
   {
	  try
	  {
		 Session s = this.getSession();
		 if (s == null) // we were not able to make the connection, warn user
		 {
		   ta.append("Unable to create a session");
		   return;
		 }
		 Database db = s.getDatabase( getParameter("serverName"), getParameter("dbName"));
		 ta.append("Title of the current database is \n   " + db.getTitle() + "\n\n");
		 ta.append("(This Java Applet is inserted in this document by the jvapplet C API sample.)\n");
	  }
	  catch (NotesException e)
	  {
		 e.printStackTrace();
	  }
   }   
}
