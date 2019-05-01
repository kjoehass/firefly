<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.3.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="250" name="Descript" color="7" fill="1" visible="yes" active="yes"/>
<layer number="251" name="SMDround" color="7" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="Wurth_Elektronik_Electromechanic_Input-Output_Connectors_rev18b" urn="urn:adsk.eagle:library:488835">
<description>&lt;BR&gt;Wurth Elektronik - Input/Output Connectors &lt;br&gt;&lt;Hr&gt;
&lt;BR&gt;&lt;BR&gt; 
&lt;TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0&gt;
&lt;TR&gt;   
&lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;&lt;BR&gt;&lt;br&gt;
      &amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp; &amp;nbsp;&lt;BR&gt;
       &lt;BR&gt;
       &lt;BR&gt;
       &lt;BR&gt;&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
&lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;&lt;br&gt;
      -----&lt;BR&gt;
      -----&lt;BR&gt;
      -----&lt;BR&gt;
      -----&lt;BR&gt;
      -----&lt;BR&gt;&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt; &lt;FONT FACE=ARIAL SIZE=3&gt;&lt;br&gt;
      ---------------------------&lt;BR&gt;
&lt;B&gt;&lt;I&gt;&lt;span style='font-size:26pt;
  color:#FF6600;'&gt;WE &lt;/span&gt;&lt;/i&gt;&lt;/b&gt;
&lt;BR&gt;
      ---------------------------&lt;BR&gt;&lt;b&gt;Würth Elektronik&lt;/b&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;&lt;br&gt;
      ---------O---&lt;BR&gt;
      ----O--------&lt;BR&gt;
      ---------O---&lt;BR&gt;
      ----O--------&lt;BR&gt;
      ---------O---&lt;BR&gt;&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
   
&lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;&lt;BR&gt;
      &amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp; &amp;nbsp;&lt;BR&gt;
       &lt;BR&gt;
       &lt;BR&gt;
       &lt;BR&gt;
       &lt;BR&gt;&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;

  &lt;TR&gt;
    &lt;TD COLSPAN=7&gt;&amp;nbsp;
    &lt;/TD&gt;
  &lt;/TR&gt;
  
&lt;/TABLE&gt;
&lt;B&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;More than you expect&lt;BR&gt;&lt;BR&gt;&lt;BR&gt;&lt;/B&gt;

&lt;HR&gt;&lt;BR&gt;
&lt;b&gt;Würth Elektronik eiSos GmbH &amp; Co. KG&lt;/b&gt;&lt;br&gt;
EMC &amp; Inductive Solutions&lt;br&gt;

Max-Eyth-Str.1&lt;br&gt;
D-74638 Waldenburg&lt;br&gt;
&lt;br&gt;
Tel: +49 (0)7942-945-0&lt;br&gt;
Fax:+49 (0)7942-945-405&lt;br&gt;
&lt;br&gt;
&lt;a href="http://www.we-online.com/web/en/electronic_components/produkte_pb/bauteilebibliotheken/eagle_4.php"&gt;www.we-online.com/eagle&lt;/a&gt;&lt;br&gt;
&lt;a href="mailto:libraries@we-online.com"&gt;libraries@we-online.com&lt;/a&gt; &lt;BR&gt;&lt;BR&gt;
&lt;br&gt;&lt;HR&gt;&lt;BR&gt;
Neither Autodesk nor Würth Elektronik eiSos does warrant that this library is error-free or &lt;br&gt;
that it meets your specific requirements.&lt;br&gt;&lt;BR&gt;
Please contact us for more information.&lt;br&gt;&lt;BR&gt;&lt;br&gt;
&lt;hr&gt;
Eagle Version 6, Library Revision 2018b,2018-11-12&lt;br&gt;
&lt;HR&gt;
Copyright: Würth Elektronik</description>
<packages>
<package name="615006138421" urn="urn:adsk.eagle:footprint:3359874/2" library_version="3">
<description>WR-MJ Horizontal Plastic 6P6C Modular Jack Tab Up, 6 Pins</description>
<wire x1="-6.1" y1="7" x2="6.1" y2="7" width="0.127" layer="51"/>
<wire x1="6.1" y1="7" x2="6.1" y2="-6" width="0.127" layer="51"/>
<wire x1="6.1" y1="-6" x2="-6.1" y2="-6" width="0.127" layer="51"/>
<wire x1="-6.1" y1="-6" x2="-6.1" y2="7" width="0.127" layer="51"/>
<wire x1="-6.1" y1="7" x2="6.1" y2="7" width="0.127" layer="21"/>
<wire x1="6.1" y1="-6" x2="-6.1" y2="-6" width="0.127" layer="21"/>
<wire x1="-6.1" y1="1.5" x2="-6.1" y2="7" width="0.127" layer="21"/>
<wire x1="6.1" y1="1.5" x2="6.1" y2="7" width="0.127" layer="21"/>
<wire x1="-6.1" y1="-1.5" x2="-6.1" y2="-6" width="0.127" layer="21"/>
<wire x1="6.1" y1="-1.5" x2="6.1" y2="-6" width="0.127" layer="21"/>
<pad name="1" x="2.55" y="4.84" drill="0.9"/>
<pad name="3" x="0.51" y="4.84" drill="0.9"/>
<pad name="5" x="-1.53" y="4.84" drill="0.9"/>
<pad name="2" x="1.53" y="2.3" drill="0.9"/>
<pad name="4" x="-0.51" y="2.3" drill="0.9"/>
<pad name="6" x="-2.55" y="2.3" drill="0.9"/>
<text x="3.705" y="4.27" size="1.27" layer="51">1</text>
<text x="-3.34" y="7.99" size="1.27" layer="25">&gt;NAME</text>
<text x="-3.44" y="-7.915" size="1.27" layer="27">&gt;VALUE</text>
<text x="-4.475" y="1.605" size="1.27" layer="51">6</text>
<text x="3.705" y="4.27" size="1.27" layer="21">1</text>
<hole x="-6" y="0" drill="2.36"/>
<hole x="6" y="0" drill="2.36"/>
<polygon width="0.127" layer="39">
<vertex x="-7.5" y="7.25"/>
<vertex x="7.5" y="7.25"/>
<vertex x="7.5" y="-6.25"/>
<vertex x="-7.5" y="-6.25"/>
</polygon>
</package>
</packages>
<packages3d>
<package3d name="615006138421" urn="urn:adsk.eagle:package:3360079/2" type="box" library_version="3">
<description>WR-MJ Horizontal Plastic 6P6C Modular Jack Tab Up, 6 Pins</description>
<packageinstances>
<packageinstance name="615006138421"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="3X3" urn="urn:adsk.eagle:symbol:3359667/2" library_version="3">
<wire x1="-7.62" y1="10.16" x2="-7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="-7.62" x2="5.08" y2="-7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="-7.62" x2="5.08" y2="10.16" width="0.254" layer="94"/>
<wire x1="5.08" y1="10.16" x2="-7.62" y2="10.16" width="0.254" layer="94"/>
<wire x1="-3.48" y1="5.71" x2="-3.48" y2="4.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="4.71" x2="-3.48" y2="3.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="3.71" x2="-3.48" y2="2.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="2.71" x2="-3.48" y2="1.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="1.71" x2="-3.48" y2="0.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="0.71" x2="-3.48" y2="-0.29" width="0.254" layer="94"/>
<wire x1="-3.48" y1="-0.29" x2="-3.48" y2="-1.29" width="0.254" layer="94"/>
<wire x1="-3.48" y1="-1.29" x2="-3.48" y2="-2.29" width="0.254" layer="94"/>
<wire x1="-3.48" y1="-2.29" x2="-3.48" y2="-3.29" width="0.254" layer="94"/>
<wire x1="-3.48" y1="-3.29" x2="2.02" y2="-3.29" width="0.254" layer="94"/>
<wire x1="-3.48" y1="5.71" x2="2.02" y2="5.71" width="0.254" layer="94"/>
<wire x1="2.02" y1="5.71" x2="2.02" y2="4.21" width="0.254" layer="94"/>
<wire x1="2.02" y1="-3.29" x2="2.02" y2="-1.79" width="0.254" layer="94"/>
<wire x1="2.02" y1="4.21" x2="3.02" y2="4.21" width="0.254" layer="94"/>
<wire x1="3.02" y1="4.21" x2="3.02" y2="2.71" width="0.254" layer="94"/>
<wire x1="3.02" y1="2.71" x2="4.02" y2="2.71" width="0.254" layer="94"/>
<wire x1="2.02" y1="-1.79" x2="3.02" y2="-1.79" width="0.254" layer="94"/>
<wire x1="3.02" y1="-1.79" x2="3.02" y2="-0.29" width="0.254" layer="94"/>
<wire x1="3.02" y1="-0.29" x2="4.02" y2="-0.29" width="0.254" layer="94"/>
<wire x1="4.02" y1="-0.29" x2="4.02" y2="2.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="3.71" x2="-2.98" y2="3.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="2.71" x2="-2.98" y2="2.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="1.71" x2="-2.98" y2="1.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="0.71" x2="-2.98" y2="0.71" width="0.254" layer="94"/>
<wire x1="-3.48" y1="-0.29" x2="-2.98" y2="-0.29" width="0.254" layer="94"/>
<wire x1="-3.48" y1="-1.29" x2="-2.98" y2="-1.29" width="0.254" layer="94"/>
<text x="-7.19" y="10.802" size="1.778" layer="95">&gt;NAME</text>
<text x="-7.206" y="-9.746" size="1.778" layer="96">&gt;VALUE</text>
<pin name="6" x="-12.7" y="7.62" visible="pad" length="middle" direction="pas"/>
<pin name="5" x="-12.7" y="5.08" visible="pad" length="middle" direction="pas"/>
<pin name="4" x="-12.7" y="2.54" visible="pad" length="middle" direction="pas"/>
<pin name="3" x="-12.7" y="0" visible="pad" length="middle" direction="pas"/>
<pin name="1" x="-12.7" y="-5.08" visible="pad" length="middle" direction="pas"/>
<pin name="2" x="-12.7" y="-2.54" visible="pad" length="middle" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="615006138421" urn="urn:adsk.eagle:component:3360283/2" prefix="J" uservalue="yes" library_version="3">
<description>&lt;b&gt;WR-MJ Horizontal Plastic 6P6C Modular Jack Tab Up, 6 Pins&lt;/b&gt;=&gt;Code : Con_I-O_MJ-H_Plastic_Up_615006138421
&lt;br&gt;&lt;a href="http://katalog.we-online.de/media/images/eican/Con_I-O_MJ-H_Plastic_Up_6150xx138421_pf2.jpg" title="Enlarge picture"&gt;
&lt;img src="http://katalog.we-online.de/media/thumbs2/eican/thb_Con_I-O_MJ-H_Plastic_Up_6150xx138421_pf2.jpg" width="320"&gt;&lt;/a&gt;&lt;p&gt;
&lt;p&gt;Details see: &lt;a href="http://katalog.we-online.de/en/em/MJ_HORIZONTAL_PLASTIC_6PXC_TAB_UP"&gt;http://katalog.we-online.de/en/em/MJ_HORIZONTAL_PLASTIC_6PXC_TAB_UP&lt;/a&gt;

Created 2014-07-10, Karrer Zheng&lt;br&gt;
2014 (C) Würth Elektronik</description>
<gates>
<gate name="G$1" symbol="3X3" x="0" y="0"/>
</gates>
<devices>
<device name="" package="615006138421">
<connects>
<connect gate="G$1" pin="1" pad="6"/>
<connect gate="G$1" pin="2" pad="5"/>
<connect gate="G$1" pin="3" pad="4"/>
<connect gate="G$1" pin="4" pad="3"/>
<connect gate="G$1" pin="5" pad="2"/>
<connect gate="G$1" pin="6" pad="1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:3360079/2"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-samtec" urn="urn:adsk.eagle:library:184">
<description>&lt;b&gt;Samtec Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="TSW-104-XX-G-S" library_version="2">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST HEADER&lt;/b&gt;&lt;p&gt;
Source: Samtec TSW.pdf</description>
<wire x1="-5.209" y1="1.155" x2="5.209" y2="1.155" width="0.2032" layer="21"/>
<wire x1="5.209" y1="1.155" x2="5.209" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="5.209" y1="-1.155" x2="-5.209" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="-5.209" y1="-1.155" x2="-5.209" y2="1.155" width="0.2032" layer="21"/>
<pad name="1" x="3.81" y="0" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<pad name="2" x="1.27" y="0" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<pad name="3" x="-1.27" y="0" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<pad name="4" x="-3.81" y="0" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<text x="3.552" y="-2.498" size="1.1" layer="21" font="vector" rot="SR0">1</text>
<text x="-5.715" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="6.985" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-4.16" y1="-0.35" x2="-3.46" y2="0.35" layer="51"/>
<rectangle x1="-1.62" y1="-0.35" x2="-0.92" y2="0.35" layer="51"/>
<rectangle x1="0.92" y1="-0.35" x2="1.62" y2="0.35" layer="51"/>
<rectangle x1="3.46" y1="-0.35" x2="4.16" y2="0.35" layer="51"/>
</package>
<package name="TSW-104-08-G-S-RA" library_version="2">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST HEADER&lt;/b&gt;&lt;p&gt;
Source: Samtec TSW.pdf</description>
<wire x1="-5.209" y1="-2.046" x2="5.209" y2="-2.046" width="0.2032" layer="21"/>
<wire x1="5.209" y1="-2.046" x2="5.209" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="5.209" y1="-0.106" x2="-5.209" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="-5.209" y1="-0.106" x2="-5.209" y2="-2.046" width="0.2032" layer="21"/>
<pad name="1" x="3.81" y="1.524" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<pad name="2" x="1.27" y="1.524" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<pad name="3" x="-1.27" y="1.524" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<pad name="4" x="-3.81" y="1.524" drill="1" diameter="1.5" shape="octagon" rot="R180"/>
<text x="-5.715" y="-7.62" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="6.985" y="-7.62" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<text x="5.092" y="1.152" size="1.1" layer="21" font="vector" rot="SR0">1</text>
<rectangle x1="-4.064" y1="0" x2="-3.556" y2="1.778" layer="51"/>
<rectangle x1="-1.524" y1="0" x2="-1.016" y2="1.778" layer="51"/>
<rectangle x1="1.016" y1="0" x2="1.524" y2="1.778" layer="51"/>
<rectangle x1="3.556" y1="0" x2="4.064" y2="1.778" layer="51"/>
<rectangle x1="-4.064" y1="-7.89" x2="-3.556" y2="-2.04" layer="21"/>
<rectangle x1="-1.524" y1="-7.89" x2="-1.016" y2="-2.04" layer="21"/>
<rectangle x1="1.016" y1="-7.89" x2="1.524" y2="-2.04" layer="21"/>
<rectangle x1="3.556" y1="-7.89" x2="4.064" y2="-2.04" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="MPINV" library_version="2">
<text x="-1.27" y="1.27" size="1.778" layer="96">&gt;VALUE</text>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<rectangle x1="0" y1="-0.254" x2="1.778" y2="0.254" layer="94"/>
<pin name="1" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="MPIN" library_version="2">
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<rectangle x1="0" y1="-0.254" x2="1.778" y2="0.254" layer="94"/>
<pin name="1" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="TSW-104-*-G-S" prefix="X" library_version="2">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST HEADER&lt;/b&gt;&lt;p&gt;
Source: Samtec TSW.pdf</description>
<gates>
<gate name="-1" symbol="MPINV" x="-1.27" y="2.54" addlevel="always"/>
<gate name="-2" symbol="MPIN" x="-1.27" y="0" addlevel="always"/>
<gate name="-3" symbol="MPIN" x="-1.27" y="-2.54" addlevel="always"/>
<gate name="-4" symbol="MPIN" x="-1.27" y="-5.08" addlevel="always"/>
</gates>
<devices>
<device name="" package="TSW-104-XX-G-S">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
<connect gate="-3" pin="1" pad="3"/>
<connect gate="-4" pin="1" pad="4"/>
</connects>
<technologies>
<technology name="07">
<attribute name="MF" value="Samtec Inc." constant="no"/>
<attribute name="MPN" value="TSW-104-07-G-S" constant="no"/>
<attribute name="OC_FARNELL" value="" constant="no"/>
<attribute name="OC_NEWARK" value="" constant="no"/>
</technology>
<technology name="08">
<attribute name="MF" value="Samtec Inc." constant="no"/>
<attribute name="MPN" value="TSW-104-07-G-S" constant="no"/>
<attribute name="OC_FARNELL" value="" constant="no"/>
<attribute name="OC_NEWARK" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="-S-RA" package="TSW-104-08-G-S-RA">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
<connect gate="-3" pin="1" pad="3"/>
<connect gate="-4" pin="1" pad="4"/>
</connects>
<technologies>
<technology name="08">
<attribute name="MF" value="Samtec Inc." constant="no"/>
<attribute name="MPN" value="TSW-104-08-G-S-RA" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0.508" drill="0">
<clearance class="0" value="0.508"/>
</class>
</classes>
<parts>
<part name="J1" library="Wurth_Elektronik_Electromechanic_Input-Output_Connectors_rev18b" library_urn="urn:adsk.eagle:library:488835" deviceset="615006138421" device="" package3d_urn="urn:adsk.eagle:package:3360079/2"/>
<part name="J2" library="Wurth_Elektronik_Electromechanic_Input-Output_Connectors_rev18b" library_urn="urn:adsk.eagle:library:488835" deviceset="615006138421" device="" package3d_urn="urn:adsk.eagle:package:3360079/2"/>
<part name="X1" library="con-samtec" library_urn="urn:adsk.eagle:library:184" deviceset="TSW-104-*-G-S" device="" technology="07"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="J1" gate="G$1" x="48.26" y="50.8" smashed="yes">
<attribute name="NAME" x="41.07" y="61.602" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.054" y="41.054" size="1.778" layer="96"/>
</instance>
<instance part="J2" gate="G$1" x="48.26" y="27.94" smashed="yes">
<attribute name="NAME" x="41.07" y="38.742" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.054" y="18.194" size="1.778" layer="96"/>
</instance>
<instance part="X1" gate="-1" x="19.05" y="53.34" smashed="yes" rot="R180">
<attribute name="VALUE" x="25.4" y="62.23" size="1.778" layer="96" rot="R180"/>
<attribute name="NAME" x="16.51" y="54.102" size="1.524" layer="95" rot="R180"/>
</instance>
<instance part="X1" gate="-2" x="19.05" y="55.88" smashed="yes" rot="R180">
<attribute name="NAME" x="16.51" y="56.642" size="1.524" layer="95" rot="R180"/>
</instance>
<instance part="X1" gate="-3" x="19.05" y="48.26" smashed="yes" rot="R180">
<attribute name="NAME" x="16.51" y="49.022" size="1.524" layer="95" rot="R180"/>
</instance>
<instance part="X1" gate="-4" x="19.05" y="25.4" smashed="yes" rot="R180">
<attribute name="NAME" x="16.51" y="26.162" size="1.524" layer="95" rot="R180"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="BLK" class="0">
<segment>
<pinref part="J1" gate="G$1" pin="5"/>
<wire x1="21.59" y1="55.88" x2="33.02" y2="55.88" width="0.1524" layer="91"/>
<pinref part="J2" gate="G$1" pin="5"/>
<wire x1="33.02" y1="55.88" x2="35.56" y2="55.88" width="0.1524" layer="91"/>
<wire x1="35.56" y1="33.02" x2="33.02" y2="33.02" width="0.1524" layer="91"/>
<wire x1="33.02" y1="33.02" x2="33.02" y2="55.88" width="0.1524" layer="91"/>
<junction x="33.02" y="55.88"/>
<label x="22.86" y="55.88" size="1.778" layer="95"/>
<pinref part="X1" gate="-2" pin="1"/>
</segment>
</net>
<net name="RED" class="0">
<segment>
<pinref part="J2" gate="G$1" pin="4"/>
<wire x1="35.56" y1="30.48" x2="30.48" y2="30.48" width="0.1524" layer="91"/>
<wire x1="30.48" y1="30.48" x2="30.48" y2="53.34" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="4"/>
<wire x1="30.48" y1="53.34" x2="35.56" y2="53.34" width="0.1524" layer="91"/>
<wire x1="30.48" y1="53.34" x2="21.59" y2="53.34" width="0.1524" layer="91"/>
<junction x="30.48" y="53.34"/>
<label x="22.86" y="53.34" size="1.778" layer="95"/>
<pinref part="X1" gate="-1" pin="1"/>
</segment>
</net>
<net name="Y1" class="0">
<segment>
<pinref part="X1" gate="-3" pin="1"/>
<pinref part="J1" gate="G$1" pin="2"/>
<wire x1="21.59" y1="48.26" x2="35.56" y2="48.26" width="0.1524" layer="91"/>
<label x="22.86" y="48.26" size="1.778" layer="95"/>
</segment>
</net>
<net name="Y2" class="0">
<segment>
<pinref part="X1" gate="-4" pin="1"/>
<pinref part="J2" gate="G$1" pin="2"/>
<wire x1="21.59" y1="25.4" x2="35.56" y2="25.4" width="0.1524" layer="91"/>
<label x="22.86" y="25.4" size="1.778" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
<errors>
<approved hash="113,1,42.3545,53.6077,J1,,,,,"/>
<approved hash="113,1,42.3545,30.7477,J2,,,,,"/>
</errors>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
