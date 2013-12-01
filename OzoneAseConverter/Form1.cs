/*
* Ozone - PSP Edition
* Copyright (C) 2009-2013 Ignacio Sanchez

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see http://www.gnu.org/licenses/
*
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using XtraceLib;

namespace OzoneAseConverter
{
    public partial class Form1 : Form
    {
        struct VerticeTemporal
        {
            public float x, y, z;
        }

        struct TexturaTemporal
        {
            public float u, v;
        }

        struct NormalTemporal
        {
            public float nx, ny, nz;
        }

        struct Triangulo
        {
            public VerticeTemporal vert1;
            public VerticeTemporal vert2;
            public VerticeTemporal vert3;

            public TexturaTemporal tex1;
            public TexturaTemporal tex2;
            public TexturaTemporal tex3;

            public NormalTemporal nor1;
            public NormalTemporal nor2;
            public NormalTemporal nor3;

            public Triangulo(VerticeTemporal v1, VerticeTemporal v2, VerticeTemporal v3)
            {
                vert1 = v1;
                vert2 = v2;
                vert3 = v3;

                tex1 = new TexturaTemporal();
                tex2 = new TexturaTemporal();
                tex3 = new TexturaTemporal();

                nor1 = new NormalTemporal();
                nor2 = new NormalTemporal();
                nor3 = new NormalTemporal();
            }
        }

        private bool m_bIsValidFile;

        private VerticeTemporal[] m_VerticesArrayTemp;
        private TexturaTemporal[] m_VerticesTexTemp;

        private System.Globalization.CultureInfo ci = new System.Globalization.CultureInfo("en-US");

        private Triangulo[] m_ArrayTriangulos;


        //--------------------------------------------------------------------
        // Función:    Form1
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 20:07:36
        //--------------------------------------------------------------------
        public Form1()
        {          
            InitializeComponent();
        }


        //--------------------------------------------------------------------
        // Función:    button1_Click
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 20:06:48
        //--------------------------------------------------------------------
        private void button1_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
        }


        //--------------------------------------------------------------------
        // Función:    button2_Click
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 20:07:00
        //--------------------------------------------------------------------
        private void button2_Click(object sender, EventArgs e)
        {
            saveFileDialog1.ShowDialog();
        }


        //--------------------------------------------------------------------
        // Función:    openFileDialog1_FileOk
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 20:07:12
        //--------------------------------------------------------------------
        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            m_bIsValidFile = true;

            StreamReader sr = new StreamReader(openFileDialog1.FileName);

            String currentLine = sr.ReadLine();

            try
            {
                while (currentLine.CompareTo("*GEOMOBJECT {") != 0)
                {
                    currentLine = sr.ReadLine();
                }

            }
            catch (Exception ex)
            {
                m_bIsValidFile = false;
                textBox1.Text = "Archivo no válido";
                Xtrace.Log("#!No se pudo encontrar un GEOMOBJECT");
                sr.Close();
                return;
            }

            ReadObject(sr);

            sr.Close();

            if (m_bIsValidFile)
            {
                textBox1.Text = openFileDialog1.FileName;
            }          
        }


        //--------------------------------------------------------------------
        // Función:    SaveObject
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 22:58:23
        //--------------------------------------------------------------------
        private void SaveObject(Stream st)
        {
            BinaryWriter br = new BinaryWriter(st);

            Xtrace.Log("#_Grabando el fichero");
            
            if (checkBox1.Checked)
            {
                br.Write((uint)(0));
                br.Write((uint)(0));
            }

            br.Write((uint)(m_ArrayTriangulos.Length));
            br.Write((uint)(m_ArrayTriangulos.Length * 3));

            foreach (Triangulo t in m_ArrayTriangulos)
            {                
                br.Write((ushort)(65535.0f * Clamp(t.tex1.u)));
                br.Write((ushort)(65535.0f * (1.0f - Clamp(t.tex1.v))));
                if (checkBox1.Checked)
                {
                    br.Write(t.nor1.nx);
                    br.Write(t.nor1.nz);
                    br.Write(-t.nor1.ny);
                }
                br.Write(t.vert1.x);
                br.Write(t.vert1.z);
                br.Write(-t.vert1.y);
                
                br.Write((ushort)(65535.0f * Clamp(t.tex2.u)));
                br.Write((ushort)(65535.0f * (1.0f - Clamp(t.tex2.v))));
                if (checkBox1.Checked)
                {
                    br.Write(t.nor2.nx);
                    br.Write(t.nor2.nz);
                    br.Write(-t.nor2.ny);
                }
                br.Write(t.vert2.x);
                br.Write(t.vert2.z);
                br.Write(-t.vert2.y);
                

                br.Write((ushort)(65535.0f * Clamp(t.tex3.u)));
                br.Write((ushort)(65535.0f * (1.0f - Clamp(t.tex3.v))));
                if (checkBox1.Checked)
                {
                    br.Write(t.nor3.nx);
                    br.Write(t.nor3.nz);
                    br.Write(-t.nor3.ny);
                }
                br.Write(t.vert3.x);
                br.Write(t.vert3.z);
                br.Write(-t.vert3.y);   
                
            }

            Xtrace.Log("#_Fichero grabado");

            br.Close();
        }


        //--------------------------------------------------------------------
        // Función:    ReadObject
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 21:01:35
        //--------------------------------------------------------------------
        private void ReadObject(StreamReader sr)
        {
            String currentLine = sr.ReadLine();
            
            while (!currentLine.Contains("*MESH_NUMVERTEX"))
            {
                currentLine = sr.ReadLine();
            }

            currentLine = FormatString(currentLine);
            String[] parse = currentLine.Split(' ');

            m_VerticesArrayTemp = new VerticeTemporal[Int32.Parse(parse[1])];

            Xtrace.Log("Vértices: {0}", m_VerticesArrayTemp.Length);

            while (!currentLine.Contains("*MESH_NUMFACES"))
            {
                currentLine = sr.ReadLine();
            }

            currentLine = FormatString(currentLine);
            parse = currentLine.Split(' ');

            m_ArrayTriangulos = new Triangulo[Int32.Parse(parse[1])];

            Xtrace.Log("Triángulos: {0}", m_ArrayTriangulos.Length);

            while (!currentLine.Contains("*MESH_VERTEX "))
            {
                currentLine = sr.ReadLine();
            }            

            int i = 0;

            while (currentLine.Contains("*MESH_VERTEX "))
            {
                currentLine = FormatString(currentLine);
                parse = currentLine.Split(' ');
                m_VerticesArrayTemp[i].x = float.Parse(parse[2], System.Globalization.NumberStyles.Float, ci);
                m_VerticesArrayTemp[i].y = float.Parse(parse[3], System.Globalization.NumberStyles.Float, ci);
                m_VerticesArrayTemp[i].z = float.Parse(parse[4], System.Globalization.NumberStyles.Float, ci);
                Xtrace.Log("Vert {0}: ({1}, {2}, {3})", i, m_VerticesArrayTemp[i].x, m_VerticesArrayTemp[i].y,
                   m_VerticesArrayTemp[i].z);
                currentLine = sr.ReadLine();
                i++;
            }

            while (!currentLine.Contains("*MESH_FACE "))
            {
                currentLine = sr.ReadLine();
            }

            i = 0;
            
            while (currentLine.Contains("*MESH_FACE "))
            {
                currentLine = FormatString(currentLine);
                parse = currentLine.Split(' ');
                int a = Int32.Parse(parse[3]);
                int b = Int32.Parse(parse[5]);
                int c = Int32.Parse(parse[7]);

                m_ArrayTriangulos[i].vert1 = m_VerticesArrayTemp[a];
                m_ArrayTriangulos[i].vert2 = m_VerticesArrayTemp[b];
                m_ArrayTriangulos[i].vert3 = m_VerticesArrayTemp[c];

                Xtrace.Log("Tri {0}: ({1}, {2}, {3})", i, a, b, c);
                currentLine = sr.ReadLine();
                i++;
            }

            while (!currentLine.Contains("*MESH_NUMTVERTEX"))
            {
                currentLine = sr.ReadLine();
            }

            currentLine = FormatString(currentLine);
            parse = currentLine.Split(' ');

            m_VerticesTexTemp = new TexturaTemporal[Int32.Parse(parse[1])];
            

            while (!currentLine.Contains("*MESH_TVERT "))
            {
                currentLine = sr.ReadLine();
            }


            i = 0;

            while (currentLine.Contains("*MESH_TVERT "))
            {
                currentLine = FormatString(currentLine);
                parse = currentLine.Split(' ');
                m_VerticesTexTemp[i].u = float.Parse(parse[2], System.Globalization.NumberStyles.Float, ci);
                m_VerticesTexTemp[i].v = float.Parse(parse[3], System.Globalization.NumberStyles.Float, ci);
                Xtrace.Log("Tex {0}: ({1}, {2})", i, m_VerticesTexTemp[i].u, m_VerticesTexTemp[i].v);
                currentLine = sr.ReadLine();
                i++;
            }


            while (!currentLine.Contains("*MESH_TFACE "))
            {
                currentLine = sr.ReadLine();
            }

            i = 0;

            while (currentLine.Contains("*MESH_TFACE "))
            {
                currentLine = FormatString(currentLine);
                parse = currentLine.Split(' ');

                m_ArrayTriangulos[i].tex1 = m_VerticesTexTemp[Int32.Parse(parse[2])];
                m_ArrayTriangulos[i].tex2 = m_VerticesTexTemp[Int32.Parse(parse[3])];
                m_ArrayTriangulos[i].tex3 = m_VerticesTexTemp[Int32.Parse(parse[4])];
                currentLine = sr.ReadLine();
                i++;
            }

            if (this.checkBox1.Checked)
            {
                while (!currentLine.Contains("*MESH_VERTEXNORMAL "))
                {
                    currentLine = sr.ReadLine();
                }

                i = 0;
                int t = 0;

                while (currentLine.Contains("*MESH_VERTEXNORMAL ") || currentLine.Contains("*MESH_FACENORMAL "))
                {
                    if (currentLine.Contains("*MESH_FACENORMAL "))
                    {
                       currentLine = sr.ReadLine();
                       continue;
                    }

                    currentLine = FormatString(currentLine);
                    parse = currentLine.Split(' ');

                    NormalTemporal n = new NormalTemporal();

                    n.nx = float.Parse(parse[2], System.Globalization.NumberStyles.Float, ci);
                    n.ny = float.Parse(parse[3], System.Globalization.NumberStyles.Float, ci);
                    n.nz = float.Parse(parse[4], System.Globalization.NumberStyles.Float, ci);

                    Xtrace.Log("Nor {0}: ({1}, {2}, {3})", (i * 3) + t, n.nx, n.ny, n.nz);

                    switch (t)
                    {
                        case 0:
                        {
                            m_ArrayTriangulos[i].nor1 = n;
                            break;
                        }
                        case 1:
                        {
                            m_ArrayTriangulos[i].nor2 = n;
                            break;
                        }
                        case 2:
                        {
                            m_ArrayTriangulos[i].nor3 = n;
                            break;
                        }
                    }

                    t++;

                    if (t == 3)
                    {
                       i++;
                       t = 0;
                    }

                    currentLine = sr.ReadLine();
                }
            }
        }


        //--------------------------------------------------------------------
        // Función:    saveFileDialog1_FileOk
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 20:07:16
        //--------------------------------------------------------------------
        private void saveFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            if (m_bIsValidFile)
                SaveObject(saveFileDialog1.OpenFile());
        }


        //--------------------------------------------------------------------
        // Función:    FormatString
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 22:15:17
        //--------------------------------------------------------------------
        private String FormatString(String str)
        {
            str = str.Replace('\t', ' ');

            while (str.Contains("  "))
            {
                str = str.Replace("  ", " ");
            }

            return str.Trim();
        }

        //--------------------------------------------------------------------
        // Función:    Form1_Load
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 21:09:54
        //--------------------------------------------------------------------
        private void Form1_Load(object sender, EventArgs e)
        {
            Xtrace.Init("ASE Converter", true);         

            m_bIsValidFile = false;
        }


        //--------------------------------------------------------------------
        // Función:    Form1_FormClosing
        // Propósito:  
        // Fecha:      miércoles, 08 de noviembre de 2006, 21:12:58
        //--------------------------------------------------------------------
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Xtrace.End();
        }


        //--------------------------------------------------------------------
        // Función:    Clamp
        // Creador:    Nacho (AMD)
        // Fecha:      Saturday  10/02/2007  17:21:26
        //--------------------------------------------------------------------
        private float Clamp(float value)
        {
            if (value < 0.0f)
                return 0.0f;
            else if (value > 1.0f)
                return 1.0f;
            else
                return value;
        }
    }
}