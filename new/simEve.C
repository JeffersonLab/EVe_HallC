{
  #include <TGClient.h>
  gSystem->Load("libGeom");
  gSystem->Load("libEve.so");
  

  int nrun, nev;
  int found = 0;
  const char** path = 0;
  char filename[300];  
  FILE *fd;
  const char* paths[] = {  
    "/u/home/davispbr/hcana-copy/Eve-2/new/",
    "./",
    0
  };
  while( found==0 ) {
    cout << "Run number?\n";
    cin >> nrun;
    if( nrun<0 ) break;
   
    path=paths;
    
    while ( path && *path ) {
      sprintf(filename,"%s/e04007_det_%d.root",*path,nrun);
      fd = fopen(filename,"r");
      if (fd != NULL) {
	found = 1;
	cout << "Will analyze file "<<filename<<endl;
	break;
      }
      path++;
    }
    if ( (!path || !*path) && !found ) {
      cout << "File not found.  Try again, or -1 to quit.\n";
    }
  }
  if(nrun<0) return;
  cout << "Opening file "<<filename<<endl;


  Eve *sim = new Eve(gClient->GetRoot(),950,678);  
  sim->initRun(filename);
  
}
