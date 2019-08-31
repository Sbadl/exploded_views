developed with:
SAMSON 0.7.0
Visual Studio 15 Win10SDK (PCA Explosion does NOT work with Visual Studio 17 Win10SDK)
Qt 5.10.1

Features:
Guided Explosion:
	Spherical Explosions & PCA Explosion:
		@PCA selection of component for explosion
		Parallel, Peeling and Sequential Explosion
		per Chain, Residue, Backbone/Sidechain, Atom
		Hierarchical (e.g. Atoms of Chain, or Backbone/Sidechain of Residue)
		only selected children explode (if none are selected then all explode)
	
Path Explosions:
	Group Selection
	Paths per Group
	Explosion along Path
	Move Path after creation
	hide arrows
	
	Proper arrow drawing:
		if(no arrow drawn)
			draw arrow
		else
			always explode to limit
			center view to explosion group
			draw arrow

draw explosion arrows
automated explosion
Ease Functions
Change of .pdb file without restart of SAMSON