function formatPlot()
    grid on;
    set(gca,'GridLineStyle','--');
    set(gca,'linewidth',0.7,'fontsize',7);
    set(gcf,'unit','centimeters','position',[10,10,8,5]);
    lines = findobj(gca, 'Type', 'Line');
    set(lines, 'LineWidth', 1.2);
end
