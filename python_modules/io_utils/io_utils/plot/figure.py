import matplotlib.pyplot as plt

def create_fig(fig_size = (14, 9)): # figure size be 12 to 7.5 inches
    from matplotlib.ticker import AutoMinorLocator
    fig, axes = plt.subplots(figsize = fig_size, constrained_layout=True) # create axes and subplots
    axes = plt.gca() # gets current axes as object to alter
    # adapt size of tick lines to sides
    axes.tick_params(axis = 'both', labelsize = 'large')
    # create dashed line every N units in color "grey" = "CCCCC"
    axes.grid(which = 'major', color = '#CCCCCC', linestyle = '--')
    axes.xaxis.set_minor_locator(AutoMinorLocator(5))
    # create M - 1 dotted lines between two major lines in grey
    axes.grid(which = 'minor', color = '#CCCCCC', linestyle = ':')
    axes.yaxis.set_minor_locator(AutoMinorLocator(5))
    # if abs(x) <= 1e-03 or abs(x) >= 1e+03: x is formatted in scientifical notation
    axes.ticklabel_format(style = 'sci', axis = 'both', scilimits = (-3, 3),
                          useMathText = True)
    # return object so that is can be modified further
    return fig, axes

def create_fig_withoutgrid(fig_size = (14, 9)):
    # analog to func above
    fig, axes = plt.subplots(figsize = fig_size, constrained_layout=True)
    axes = plt.gca()
    axes.ticklabel_format(style = 'sci', axis = 'both', scilimits = (-3, 3),
                          useMathText = True)
    return fig, axes

def layout_on_axis(ax):
    from matplotlib.ticker import AutoMinorLocator
    # Größe der Tick-Labels
    ax.tick_params(axis='both', labelsize='large')
    # Major Grid
    ax.grid(which='major', color='#CCCCCC', linestyle='--')
    ax.xaxis.set_minor_locator(AutoMinorLocator(5))
    ax.yaxis.set_minor_locator(AutoMinorLocator(5))
    # Minor Grid
    ax.grid(which='minor', color='#CCCCCC', linestyle=':')
    # Wissenschaftliche Notation
    ax.ticklabel_format(style='sci', axis='both', scilimits=(-3, 3), useMathText=True)

def set_std_fontsize (factor = 1, **kwargs):
    plt.rcParams['font.size'] = kwargs.get("font_size", 20) * factor
    plt.rcParams['axes.labelsize'] = kwargs.get("axes_labelsize", 22) * factor
    plt.rcParams['xtick.labelsize'] = kwargs.get("xtick_labelsize", 20) * factor
    plt.rcParams['ytick.labelsize'] = kwargs.get("ytick_labelsize", 20) * factor
    plt.rcParams['legend.fontsize'] = kwargs.get("legend_fontsize", 20) * factor
    plt.rcParams['figure.titlesize'] = kwargs.get("figure_titlesize", 24) * factor

def activate_latex(*packages):
    plt.rcParams['text.usetex'] = True
    # import helpful usepackages
    for package in packages:
        plt.rcParams['text.latex.preamble'] += r'\usepackage{' + package + '}\n'


