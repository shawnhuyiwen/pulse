## What are some possible physiology engine applications?
<center>
<table border="0">
<tr>
    <td><center><b>Virtual/Augmented Environments</b></center></td>
    <td><center><b>Manikin-Based Simulations</b></center></td>
  <td><center><b>Clinical Explorations</b></center></td>
</tr>
<tr>
    <td><img src="./Images/PulseVR.png" width="365"></td>
    <td><img src="./Images/PulseClinician.png" width="365"></td>
  <td><img src="./Images/PulseProgrammer.png" width="365"></td>
</tr>
</table>
</center>

There is a wide range of potential applications, a few include:
- Powering serious games for medical education and training
- Producing responsive physiology in real time for manikin training
- Integrating higher fidelity single-system or sub-system models to understand full-body physiologic response
- Connecting in-silico patients for hardware-in-the-loop (HIL) testing of medical devices
- Providing simulations and visualizations for teaching and education
- Pairing with virtual surgery planning/rehearsal

## Featured Applications Leveraging Pulse

### Pulse Physiology Explorer

The Pulse Physiology Engine is a powerful tool in computing the physiological responses to acute injury and treatment.
However, without a visualization tool the information is difficult to understand.
As part of the Kitware physiology repository, we have developed a visualization tool built on Qt to provide a way to dynamically interact with the Pulse physiology engine.

\htmlonly
<center>
  <a href="https://gitlab.kitware.com/physiology/explorer/wikis/home"><img src="./Images/PulseExplorer-Vitals.png" width="365"/></a><br>
  <em>For more information on getting and using this tool, visit our <a href="https://gitlab.kitware.com/physiology/explorer/wikis/home">Wiki</a></em>
</center>
\endhtmlonly

### Pulse Unity Asset

The Pulse Unity Asset is available for free download on the <a href="https://assetstore.unity.com/packages/tools/integration/pulse-physiology-engine-139773">Unity Asset Store</a> and the <a href="https://gitlab.kitware.com/physiology/PulseUnityAsset/releases">release page</a> of the open-source project on <a href="https://gitlab.kitware.com/physiology/PulseUnityAsset">Gitlab</a>. The Pulse Unity Asset enables the integration of the Pulse Physiology Engine into Unity applications targeting Windows, Linux and MacOS platforms.

\htmlonly
<center>
<table border="0">
<tr>
    <td><a href="https://assetstore.unity.com/packages/tools/integration/pulse-physiology-engine-139773"><img src="./Images/Unity1.jpg" width="365"></a></td>
    <td><a href="https://assetstore.unity.com/packages/tools/integration/pulse-physiology-engine-139773"><img src="./Images/Unity2.jpg" width="365"></a></td>
</tr>
</table>
<em>
Our user manual offers in-depth details of all the Pulse asset functionality. It is included in the unity package and can be found on our <a href="https://gitlab.kitware.com/physiology/PulseUnityAsset/releases">release page</a>.
</em>
</center>
\endhtmlonly

### Trauma Simulator

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    An immersive virtual reality emergency medicine training simulator for military medical personnel. This simulator uses the Unity game engine and Pulse to provide dynamic physiological feedback on the patient's condition  from a wide range of injuries and treatments.
    <img src="./Images/Features/TraumaSimulator.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="http://www.exonicus.com/"><img src="./Images/Features/ExonicusLogo.png" width="200"></a><br><br>
      <a href="https://www.mamc.health.mil/"><img src="./Images/Features/MadiganLogo.png" width="200"></a><br><br>
      <a href="https://www.tatrc.org/"><img src="./Images/Features/TATRCLogo.png" width="100"></a><br><br>
      <a href="https://www.kitware.com/"><img src="./Images/Features/KitwareLogo.png" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://militarysimulation.training/technology/exonicus-kitware-trauma-simulator-%ef%bb%bf/">Military Simulation & Training Magazine Article</a><br>
      &bull; <a href="https://blog.kitware.com/exonicus-and-kitware-collaborate-to-develop-a-trauma-simulator-using-the-pulse-physiology-engine/">Kitware Blog Post</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Closed-Loop Physiology Management System

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    A system for investigating closed-loop physiology management for critical care with in-silico patients. Closed-Loop Assistants (CLAs) are designed to leverage medical device interfaces to add computers/algorithms to the clinical care loop to aid indecision-making and to implement the automatic application of interventions. 
    <br><img src="./Images/Features/CLA.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://www.bucknell.edu/"><img src="./Images/Features/BucknellLogo.png" width="200"></a><br><br>
      <a href="https://www.geisinger.edu/"><img src="./Images/Features/GeisingerLogo.png" width="200"></a><br><br>
      <a href="https://www.kitware.com/"><img src="./Images/Features/KitwareLogo.png" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://blog.kitware.com/pulse-physiology-used-for-testing-and-validation-of-a-closed-loop-physiology-management-system/">Kitware Blog Post</a><br>
      &bull; <a href="https://clasim.openconnectedmed.org/">CLASim Website</a><br>
      &bull; <a href="https://gitlab.com/openconnectedmed/clasim/clasim_software_only">CLASim Repository</a><br>
      &bull; <a href="https://rtg.cis.upenn.edu/mcps-workshop-2018/#program">F. Gessa, P. Asare, A. Bray, R. Clipp, and M. Poler, "Towards A Test and Validation Framework for Closed-Loop Physiology Management Systems for Critical and Perioperative Care," in Medical Cyber Physical Systems Workshop, 2018.</a><br>
      &bull; <a href="http://arajhans.github.io/files/proceedings/MT-CPS-2019/MT-CPS_2019_paper_7.pdf">Farooq Gessa, Philip Asare1, Dikendra Karki1, Aaron Bray, Rachel B. Clipp, Mark Poler, "Simulation-Based Test and Validation of Medical Cyber-Physical Systems for Critical and Perioperative Care," Workshop on Monitoring and Testing of Cyber-Physical Systems (MT-CPS).</a><br>
      &bull; <a href="https://www.stahq.org/userfiles/files/009_STA_19AM_Abstract_Karki%28464%29.pdf">Farooq  Gessa, Aaron Bray, Rachel Clipp, Philip  Asare, S. Mark Poler, "A Proof-of-Concept Framework for Testing and Validating Networked Medical Device Applications and Closed-Loop Physiology Management Systems for Critical and Perioperative Care," Society for Technology in Anesthesia (STA) Annual Meeting </a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Medical Simulation and Training Architecture (MSTA)

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    The MSTA platform provides an open standard to connect manikins, part-tasks trainers, physiology engines, and other simulation technologies to support the creation of complex training systems necessary for future force readiness efforts.
    MSTA successfully demonstrated an integrated TCCC training scenario that took a wounded virtual patient from field care to role 2 care. The training system consisted of a manikin, a custom control panel, a part task trainer, the Pulse physiology engine and an after action review engine.
    <img src="./Images/Features/MSTA.png" width="400">
    <em>This work was performed under Contract #W900KK-17-C-0041</em>
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="http://www.simquest.com"><img src="./Images/Features/SimQuestLogo.png" width="200"></a><br>
      <a href="https://www.health.mil/About-MHS/OASDHA/Defense-Health-Agency"><img src="./Images/Features/DHA.jpeg" width="200"></a><br>
      <a href="https://www.peostri.army.mil"><img src="./Images/Features/PEOSTRILogo.png" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://blog.kitware.com/pulse-physiology-used-for-testing-and-validation-of-a-closed-loop-physiology-management-system/">Kitware Blog Post</a><br>
      
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Ventilation Management Trainer

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
     A training simulator designed to mimic respiratory distress during mechanical ventilation. A simulated torso was developed and integrated with an appropriately equipped Special Medical Emergency Evacuation Device affixed with medical equipment utilized during Critical Care Air Transport Team missions. The torso includes a lung model, upper airway, and head with reproducible computerized algorithms with the ability to simulate respiratory distress. The simulator is responsive to providers' interventions in treating conditions encountered during mechanical ventilation.	 
	 Pulse is fully integrated to autonomously drive all patient responses.
    <img src="./Images/Features/VentilationTrainer.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://www.ingmarmed.com/"><img src="./Images/Features/IngMarLogo.jpg" width="200"></a><br><br>
      <a href="https://www.uchealth.com/education/c-stars/"><img src="./Images/Features/UCHealthLogo.png" width="200"></a><br><br>
	  <a href="https://www.wpafb.af.mil/afrl/"><img src="./Images/Features/ARLLogo.jpg" width="150"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://apps.dtic.mil/dtic/tr/fulltext/u2/1074448.pdf">Rodriquez Jr, Dario, Thomas Blakeman, Dina Gomaa, and Richard Branson. Advancing Mechanical Ventilation Management through Simulation. No. AFRL-SA-WP-SR-2019-0006. USAFSAM/FHE Wright-Patterson AFB United States, 2019.</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Combat Casualty Care Augmented Reality Intelligent Training System (C3ARESYS)

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    An augmented reality system to improve the realism of Combat Medics (68W) and Combat Lifesavers scenario-based training. C3ARESYS provides the opportunity to train on wounds and casualties that respond to treatments with feedback adapted to the trainee's skill level. C3ARESYS offloads work from the instructor, enabling focus on teaching rather than fixing shortcomings in casualty simulation.
    Pulse is used to provide dynamic interactions to the patient and provide physiological feedback from the patient.
    <br><img src="./Images/Features/C3ARESYS.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://soartech.com/"><img src="./Images/Features/SoarTechLogo.jpg" width="200"></a><br><br>
      <a href="https://www.ucf.edu/"><img src="./Images/Features/UCFLogo.jpg" width="200"></a><br><br>
      <a href="https://www.peostri.army.mil"><img src="./Images/Features/PEOSTRILogo.png" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://sreal.ucf.edu/wp-content/uploads/2018/11/Taylor2018aa.pdf">Taylor, Glenn, Anthony Deschamps, Alyssa Tanaka, Denise Nicholson, Gerd Bruder, Gregory Welch, and Francisco Guido-Sanz, "Augmented Reality for Tactical Combat Casualty Care Training," International Conference on Augmented Cognition. Springer, Cham, 2018.</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### The BioMojo Virtual Patient Experience (VPE)

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    An interactive, multiplayer 3D healthcare and medicine themed, STEM education product. BioMojo VPE is designed to inspire and educate youth towards careers in healthcare, clinical research and biomedical engineering through fun, challenging virtual role play, teamwork, and problem-solving. Themes include emergency medicine, preventable chronic diseases, physiology, anatomy, genomics, and pharmacotherapy.
    Players will perform (virtual) diagnostic procedures and other interactions with virtual patient avatars. Virtual patient physiology is provided by Pulse.
    <img src="./Images/Features/vpe_pharmacy.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://www.biomojo.com/"><img src="./Images/Features/BioMojoLogo.png" width="200"></a><br>
      <a href="https://www.kitware.com/"><img src="./Images/Features/KitwareLogo.png" width="200"></a><br>
      <a href="https://www.nih.gov/"><img src="./Images/Features/NIHLogo.jpg" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://biomojo.com/">BioMojo</a><br>
      
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Virtual Pediatric Airway Workbench (VPAW)

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    A surgical planning tool for subglottic stenosis that incorporates three major components.
    VPAW initiates with a CT scan of the patient and obtains a geometrical model through segmentation and surface reconstruction.
    It then employs a computational fluid dynamics (CFD) engine based on a Lattice-Boltzmann formulation to provide airflow parameters for Pulse. Pulse then provides the physiologic response due to the airflow.
    A real-time geometric authoring tool allows surgeons to edit the tracheal geometry using a haptic device as part of a surgical planning. VPAW calculates the physiologic results of each plan to be assessed by surgeons to identify the best course of action.
    <br><img src="./Images/Features/VPAW.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://www.kitware.com/"><img src="./Images/Features/KitwareLogo.png" width="200"></a><br><br>
      <a href="https://www.unc.edu/"><img src="./Images/Features/UNCLogo.jpg" width="200"></a><br><br>
      <a href="https://www.nih.gov/"><img src="./Images/Features/NIHLogo.jpg" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://blog.kitware.com/toward-patient-specific-surgical-planning-including-individualized-physiology-modeling/">Kitware Blog Post</a><br>
      &bull; <a href="https://www.spiedigitallibrary.org/conference-proceedings-of-spie/10135/101351T/Physiology-informed-virtual-surgical-planning--a-case-study-with/10.1117/12.2252510.short">L. Potter, S. Arikatla, A. Bray, J. Webb, and A. Enquobahrie, "Physiology informed virtual surgical planning: a case study with a virtual airway surgical planner and BioGears," in SPIE Medical Imaging, 2017.</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Ventriculoperitoneal Shunt Performance

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    A high-fidelity computational surrogate head model focused on the ventricular system to optimize the performance of ventriculoperitoneal shunts. The cerebrospinal fluid model is being coupled with the cerebrovascular system using the Pulse physiology engine. In addition to globally quantifying the essential cerebrovascular parameters for the local high-fidelity analysis of shunt function, Pulse also provides an invaluable training capability to teach students about the effects of elevated ICP due to hydro-cephalus on the entire body.
    
    <img src="./Images/Features/spacer.jpg" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://www.utk.edu/"><img src="./Images/Features/UTLogo.png" width="200"></a><br><br>
      <a href="https://www.nih.gov/"><img src="./Images/Features/NIHLogo.jpg" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://rdcu.be/btrMf">Bray, Aaron, Jeffrey B. Webb, Andinet Enquobahrie, Jared Vicory, Jerry Heneghan, Robert Hubal, Stephanie TerMaath, Philip Asare, and Rachel B. Clipp, "Pulse Physiology Engine: an Open-Source Software Platform for Computational Modeling of Human Medical Simulation," SN Comprehensive Clinical Medicine (2019).</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Modeling Valvular Diseases

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    Simulating three valvular conditions: aortic stenosis, aortic regurgitation, and mitral stenosis.  Pulse virtual physiology software has the potential to transform medical education by allowing medical students to learn in a consequence-free environment. Medically accurate physiology models are required to ensure that lessons learned virtually translate to the real world.
    <img src="./Images/Features/UIUCSimulation.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://illinois.edu/"><img src="./Images/Features/UIUCLogo.png" width="200"></a><br><br>
      <a href="https://medicine.uic.edu/"><img src="./Images/Features/UICOMPLogo.png" width="300"></a><br><br>
      <a href="https://www.kitware.com/"><img src="./Images/Features/KitwareLogo.png" width="200">
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://embs.papercept.net/conferences/conferences/EMBC18/program/EMBC18_ContentListWeb_2.html">Paul Ruales Rosero, Pavithra Rajeswaran, Jeffrey Webb, Aaron Bray, Thenkurussi Kesavadas. "Modelling Valvular Diseases Using a Virtual Physiology Engine." 40th Annual International Conference of the IEEE Engineering in Medicine and Biology Society. Honolulu, Hawaii. 2018.</a><br>
      &bull; <a href="https://healtheng.illinois.edu/research/simulation/virtual-physiology-engine-based-simulation-modules/">Website</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### SOFA Integration

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:1px Gainsboro; padding:3px">
    Created an interface plugin within the SOFA multi-physics simulation framework to link with the Pulse physiology engine.
    <img src="./Images/Features/GSoC.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://www.sofa-framework.org/"><img src="./Images/Features/SOFALogo.png" width="200"></a><br><br>
      <a href="https://www.kitware.com/"><img src="./Images/Features/KitwareLogo.png" width="200"><a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://github.com/sofa-framework/PulsePhysiology/commits/master">GitHub repository</a><br>
      &bull; <a href="https://blog.kitware.com/kitware-and-inria-collaborate-on-open-source-surgical-simulation-and-computational-physiology-integration/">Kitware Blog Post</a><br>
      &bull; <a href="https://medium.com/@jn.manan25/developer-at-google-summer-of-code-18-with-inria-foundation-sofa-and-pulse-da09100504d1">Developer Blog Post</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

### Modular Deployment

\htmlonly
<center>
<table color="white" border="1" bordercolor="Gainsboro" style="border-collapse: collapse">
<tr>
  <th><center>Description</center></th>
  <th><center>Team</center></th>
  <th><center>Links</center></th>
</tr>
<tr>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px">
    Pulse is deployable on low size, weight, power, and cost (SWaP-C) systems, and has been shown to run faster than real-time on several single-board computers.
    <img src="./Images/Features/SingleBoard.png" width="400">
  </td>
  <td width="30%" style="border-top:2px solid Gainsboro; padding:3px" valign="top">
    <center>
      <a href="https://www.kitware.com/"><img src="./Images/Features/KitwareLogo.png" width="200"></a>
    </center>
  </td>
  <td width="40%" valign="top" style="border-top:2px solid Gainsboro; padding:3px">
      &bull; <a href="https://blog.kitware.com/benchmarking-the-pulse-physiology-engine-on-various-single-board-computing-platforms/">Kitware Blog Post</a><br>
  </td>
</tr>
</table>
</center>
\endhtmlonly

<b>To feature your Pulse use case, please email us at kitware@kitware.com!</b>
